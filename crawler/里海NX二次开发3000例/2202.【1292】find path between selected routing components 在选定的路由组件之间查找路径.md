### 【1292】find path between selected routing components 在选定的路由组件之间查找路径

#### 代码

```cpp
    /*  
        See PR 6545989 - This does not work well when the path traverses multiple  
        assembly levels.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_route.h>  
    #include <uf_assem.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static int is_route(tag_t object, int type[3], void *user_data,  
        UF_UI_selection_p_t select)  
    {  
        if (UF_ROUTE_is_part_occ_route_part(object))  
            return UF_UI_SEL_ACCEPT;  
        else  
            return UF_UI_SEL_REJECT;  
    }  
    /* qq3123197280 */  
    static int mask_for_route_components(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_component_type, 0, 0 };  
        UF_CALL(UF_UI_set_sel_procs(select, is_route, NULL, NULL));  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_route_component(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a routing component",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_route_components,  
            NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        for (ii = 0; ii < n; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            if (type == UF_occ_instance_type)  
                UF_CALL(UF_DISP_set_highlight(UF_ASSEM_ask_part_occ_of_inst(  
                    NULL_TAG, objects[ii]), flag));  
            else  
                UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
        }  
    }  
    #define WRITE_L(X) if (X) ECHO("%s = TRUE\n", #X); \  
        else ECHO("%s = FALSE\n", #X);  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            end,  
            *route_objs,  
            start;  
        char  
            msg[133];  
        while (((start = select_a_route_component("Path start")) != NULL_TAG) &&  
            ((end = select_a_route_component("Path end")) != NULL_TAG))  
        {  
            UF_CALL(UF_ROUTE_find_path(start, end, &n, &route_objs));  
            set_highlight_object_array(n, route_objs, TRUE);  
            sprintf(msg, "found %d objects in path", n);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n, route_objs, FALSE);  
            if (n > 0) UF_free(route_objs);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码实现了以下功能：
>
> 1. 错误报告机制：定义了ECHO和report_error函数，用于输出错误信息。
> 2. 路由组件选择：定义了is_route函数，用于判断一个对象是否为路由组件，以及mask_for_route_components函数，用于设置组件选择器。
> 3. 组件选择和路径查询：定义了select_a_route_component函数，用于选择一个路由组件，以及do_it函数，用于查询组件之间的路径并高亮显示。
> 4. 初始化和终止：在ufusr函数中，初始化NX环境并调用do_it函数，最后终止NX环境。
> 5. 卸载处理：定义了ufusr_ask_unload函数，用于处理二次开发的卸载。
>
> 综上所述，该代码通过选择路由组件，查询组件之间的路径并高亮显示，实现了NX中的路径查询功能。
>
