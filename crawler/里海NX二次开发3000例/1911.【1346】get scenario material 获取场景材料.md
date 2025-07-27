### 【1346】get scenario material 获取场景材料

#### 代码

```cpp
    /*HEAD GET_SCENARIO_MATERIAL CCC UFUN */  
    #include <uf_exit.h>  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf_object_types.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_sf.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意：UF_print_syslog是V18版本新增的。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t     
            body,   
            material_tag;  
        char     
            msg[132] = { "" },  
            *mat_name;  
        UF_SF_material_prop_t   
            props;  
        UF_CALL(UF_UI_close_listing_window());  
        body = select_a_body("Select body for material query");  
        UF_CALL(UF_SF_init_matl_prop(&props));  
        UF_CALL( UF_SF_locate_material( body, &material_tag));  
        UF_CALL( UF_SF_ask_material( material_tag, &mat_name, &props));  
        UF_CALL(UF_UI_open_listing_window());      
        sprintf(msg,"Material name: %s\n", mat_name);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        UF_SF_free_matl_prop(&props);  
        UF_free( mat_name);  
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

> 这段NX二次开发代码的主要功能是查询选定体的材料属性。具体包括以下内容：
>
> 1. 定义了错误报告函数report_error，用于在函数调用失败时打印错误信息。
> 2. 定义了mask_for_bodies函数，用于设置选择过滤器，只允许选择实体。
> 3. 定义了select_a_body函数，用于弹出一个对话框让用户选择一个实体。
> 4. 定义了do_it函数，用于执行主体逻辑：关闭列表窗口调用select_a_body函数让用户选择一个实体初始化材料属性结构体查询选定实体的材料标签查询材料属性打开列表窗口并打印材料名称
> 5. 关闭列表窗口
> 6. 调用select_a_body函数让用户选择一个实体
> 7. 初始化材料属性结构体
> 8. 查询选定实体的材料标签
> 9. 查询材料属性
> 10. 打开列表窗口并打印材料名称
> 11. 定义了ufusr函数，作为NX的回调函数。在NX启动时会调用该函数，在该函数中初始化NX环境，调用do_it函数执行主体逻辑，然后终止NX环境。
> 12. 定义了ufusr_ask_unload函数，表示该模块可以立即卸载。
>
> 总的来说，这段代码通过二次开发实现了在NX中查询选定实体的材料属性的功能。用户只需选择一个实体，即可查看其材料名称和属性。
>
