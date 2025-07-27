### 【2038】report count of selected faces adjacent facesUF_MODL_ask_adjac_faces 报告所选面的相邻面数量 UF_MODL_ask_

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
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
    /* qq3123197280 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            count;  
        tag_t  
            face;  
        uf_list_p_t  
            adjacent_faces = NULL;  
        while ((face = select_a_face("Report adjacent faces")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_adjac_faces(face, &adjacent_faces));  
            UF_CALL(UF_MODL_ask_list_count(adjacent_faces, &count));  
            ECHO("%d adjacent faces\n", count);  
            if (count > 0) UF_CALL(UF_MODL_delete_list(&adjacent_faces));  
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

> 这段代码是一个NX的二次开发示例，主要实现了以下功能：
>
> 1. 定义了一个ECHO函数，用于在NX的日志窗口和系统日志中输出信息，方便调试。
> 2. 定义了一个UF_CALL宏，用于调用NX的UF函数，并自动报告错误。
> 3. 定义了一个mask_for_faces函数，用于设置选择过滤条件，只选择实体类型的面。
> 4. 定义了一个select_a_face函数，用于弹出一个对话框，让用户选择一个面，并返回该面的tag。
> 5. 定义了一个do_it函数，用于循环让用户选择一个面，然后查询并输出该面的相邻面数量。
> 6. 在ufusr函数中，初始化NX环境，调用do_it函数执行主体功能，然后终止NX环境。
> 7. 定义了一个ufusr_ask_unload函数，用于卸载用户自定义函数，返回立即卸载。
>
> 总的来说，这段代码实现了选择一个面并查询相邻面的功能，为NX的二次开发提供了一个简单的示例。
>
