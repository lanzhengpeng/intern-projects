### 【2100】report face topology 报告面拓扑

#### 代码

```cpp
    /*HEAD REPORT_FACE_TOPOLOGY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_modl_types.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我翻译如下：

注意：UF_print_syslog是V18版本新增的功能。

仅提供翻译，无需添加其他内容。 */  
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
            topo_type = 0;  
        tag_t  
            face = NULL_TAG;  
        while( ( face = select_a_face( "Pick a Face:" )) != NULL_TAG )   
        {  
            UF_CALL( UF_MODL_ask_face_topology( face, &topo_type ));  
            UF_UI_open_listing_window();  
            switch  (topo_type)  
            {  
                case UF_MODL_FLAT_TOPOLOGY:  
                  UF_UI_write_listing_window( "UF_MODL_FLAT_TOPOLOGY\n" );  
                  break;  
                case UF_MODL_CYLINDRICAL_TOPOLOGY:  
                  UF_UI_write_listing_window( "UF_MODL_CYLINDRICAL_TOPOLOGY\n" );  
                  break;  
                case UF_MODL_CONICAL_TOPOLOGY:  
                  UF_UI_write_listing_window( "UF_MODL_CONICAL_TOPOLOGY\n" );  
                  break;  
                case UF_MODL_SPHERICAL_TOPOLOGY:  
                  UF_UI_write_listing_window( "UF_MODL_SPHERICAL_TOPOLOGY\n" );  
                  break;  
                case UF_MODL_TOROIDAL_TOPOLOGY:  
                  UF_UI_write_listing_window( "UF_MODL_TOROIDAL_TOPOLOGY\n" );  
                  break;  
                case 0:  
                  UF_UI_write_listing_window( "Error, or input was a body with multiple faces \n" );  
                  break;  
                default:  
                  break;  
            }    
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

> 这段代码是用于NX二次开发，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在调用UF函数失败时输出错误信息。
> 2. 定义了一个函数mask_for_faces，用于设置选择模式，使得只能选择面特征。
> 3. 定义了一个函数select_a_face，用于提示用户选择一个面特征，并返回面特征的对象标识。
> 4. 定义了一个函数do_it，用于循环提示用户选择面特征，并使用UF_MODL_ask_face_topology函数查询面特征的拓扑类型，然后输出到列表窗口。
> 5. 定义了ufusr函数，用于初始化和终止NX Open API，并调用do_it函数。
> 6. 定义了ufusr_ask_unload函数，用于卸载用户函数模块。
>
> 通过这段代码，用户可以方便地选择NX模型中的面特征，并查询其拓扑类型，实现了面特征拓扑类型识别的功能。
>
