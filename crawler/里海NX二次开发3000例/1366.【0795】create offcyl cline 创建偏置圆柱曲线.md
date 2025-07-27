### 【0795】create offcyl cline 创建偏置圆柱曲线

#### 代码

```cpp
    /*HEAD CREATE_OFFCYL_CLINE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_drf.h>  
    #include <uf_view.h>  
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
    /* 里海 */  
    static int mask_for_arcs(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_circle_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_CIRCULAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_arc(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an arc", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_arcs, NULL, &resp,  
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
        int   
            part_units = -1;  
        tag_t  
            arc_1 = NULL_TAG,  
            arc_2 = NULL_TAG,  
            offcyl_cline = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            member_view = NULL_TAG;  
        double  
            offset_distance = 25.;  
        char  
            view_name[30] = { "" };  
        UF_DRF_object_t   
            object1,   
            object2;  
        if( NULL_TAG == part ) return;  
        UF_CALL( UF_UI_set_cursor_view ( 0 )); /* any view 译:任何观点 */  
        UF_CALL( UF_PART_ask_units( part, &part_units ));  
        if( part_units == UF_PART_ENGLISH ) offset_distance = 1.0;   
        if( (arc_1=select_an_arc( "First Arc:"))!= NULL_TAG &&  
            (arc_2=select_an_arc( "Second Arc:"))!= NULL_TAG )  
        {  
            UF_CALL( UF_DRF_init_object_structure( &object1 ));  
            UF_CALL( UF_DRF_init_object_structure( &object2 ));  
            uc1653( view_name );  
            UF_CALL( UF_VIEW_ask_tag_of_view_name( view_name, &member_view ));  
            object1.object_tag = arc_1;  
            object1.object_view_tag = member_view;  
            object1.object_assoc_type = UF_DRF_arc_center;  
            object1.object_assoc_modifier = UF_DRF_arc_center;  
            object2.object_tag = arc_2;  
            object2.object_view_tag = member_view;  
            object2.object_assoc_type = UF_DRF_arc_center;  
            object2.object_assoc_modifier = UF_DRF_arc_center;   
            UF_CALL( UF_DRF_create_offcyl_cline_off( &object1, &object2,  
                                          offset_distance, &offcyl_cline ));  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX二次开发程序，其主要功能是创建一个偏置圆柱曲线。下面是对代码的介绍：
>
> 1. 头文件引入：引入了NX Open API的相关头文件，包括UF、UF_OBJ、UF_PART、UF_DRF、UF_VIEW、UF_UI等。
> 2. 宏定义：定义了UF_CALL宏，用于检查UF函数的返回码，如果返回错误则输出错误信息。
> 3. 错误处理函数：定义了report_error函数，用于输出UF函数的错误信息。
> 4. 选择过滤函数：定义了mask_for_arcs函数，用于在UF_UI_select_with_single_dialog中过滤选择，只选择圆弧类型的对象。
> 5. 选择函数：定义了select_an_arc函数，用于弹出一个对话框让用户选择一个圆弧对象，并返回其tag。
> 6. 主要逻辑函数：定义了do_it函数，首先获取当前激活的部件和视图，然后提示用户选择两个圆弧，最后调用UF_DRF_create_offcyl_cline_off函数创建偏置圆柱曲线。
> 7. ufusr入口函数：定义了ufusr函数，作为程序的入口，首先初始化NX Open API，然后调用do_it函数执行主要逻辑，最后终止NX Open API。
> 8. 卸载函数：定义了ufusr_ask_unload函数，用于在程序卸载时立即卸载。
>
> 总体来说，这段代码实现了选择两个圆弧并创建它们之间的偏置圆柱曲线的功能，适用于NX的二次开发。
>
