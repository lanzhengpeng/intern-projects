### 【0819】create perpendicular constraint in sketch 在草图中创建垂直约束

#### 代码

```cpp
    /*HEAD CREATE_PERPENDICULAR_CONSTRAINT_IN_SKETCH CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_sket.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数，用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* 里海 */  
    static int mask_for_lines(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[2] = { { UF_line_type, 0, 0 },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_LINEAR_EDGE } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 2, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static int mask_for_datum_axes(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_datum_axis_type, 0, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* 里海 */  
    static tag_t select_a_datum_axis(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a datum axis", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_datum_axes, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static tag_t select_a_line(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a line", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_lines, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* 里海 */  
    static void make_sketch( tag_t line, tag_t datum_axis )  
    {  
        tag_t   
            sketchtag = NULL_TAG,  
            constag = NULL_TAG;  
        char   
            sket_name[MAX_ENTITY_NAME_SIZE+1]={"SKETCH_123"};  
        double   
            matrix[9]={ 1.,0.,0., 0.,1.,0., 0.,0.,1. };  
        UF_SKET_con_geom_t   
            gs[2];  
        UF_CALL(UF_SKET_initialize_sketch( sket_name, &sketchtag ));  
        UF_CALL(UF_SKET_create_sketch( sket_name,2,matrix,NULL,NULL,  
                                                      1,&sketchtag ));  
        UF_CALL(UF_SKET_add_objects( sketchtag, 1, &datum_axis ));  
        UF_CALL(UF_SKET_add_objects( sketchtag, 1, &line ));  
        gs[1].geom_tag = line;  
        gs[0].geom_tag = datum_axis;      
        UF_CALL( UF_SKET_create_geometric_constraint( sketchtag,  
                                          UF_SKET_perpendicular,   
                                                 2, gs, &constag ));  
        UF_CALL(UF_SKET_update_sketch( sketchtag ));  
        UF_CALL(UF_SKET_terminate_sketch());  
    }  
    static void do_it(void)  
    {  
        tag_t  
            line = NULL_TAG,  
            datum_axis = NULL_TAG;  
        if( ((datum_axis= select_a_datum_axis("Select Datum Axis:")) != NULL_TAG) &&  
            ((line = select_a_line("Select line:")) != NULL_TAG )   
          )   
          {  
              make_sketch( datum_axis, line );  
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

> 这段代码是一个NX二次开发示例，实现了在草图中创建垂直约束的功能。主要步骤包括：
>
> 1. 定义错误报告函数report_error，用于打印UF函数调用错误信息。
> 2. 定义两个辅助函数mask_for_lines和mask_for_datum_axes，用于设置选择过滤条件。
> 3. 定义两个选择函数select_a_line和select_a_datum_axis，用于让用户选择一条线和一条基准轴。
> 4. 定义make_sketch函数，用于创建草图，将选择的线和基准轴添加到草图中，并创建垂直约束。
> 5. do_it函数中调用选择函数，获取用户选择的线和基准轴，然后调用make_sketch创建垂直约束。
> 6. ufusr函数是NX调用的主函数，初始化后调用do_it，然后终止。
> 7. ufusr_ask_unload函数用于卸载NX Open，返回立即卸载。
>
> 通过这段代码，用户可以方便地在NX草图中创建垂直约束。
>
