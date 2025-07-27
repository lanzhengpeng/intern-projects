### 【1421】import current view into a drawing in a new part 将当前视图导入新部件中的图纸

#### 代码

```cpp
    /*HEAD IMPORT_CURRENT_VIEW_INTO_A_DRAWING_IN_A_NEW_PART CCC UFUN */  
    /** Program assumes that you have a metric part open, and some geometry is   
        displayed.  
        The program will note the current work view, and attempt to import it into  
        a newly-created part called "a_new_part".  
        Note that the part that you start with will be added as a component   
        to the newly-created assembly part.  This also happens interactively.  
        The new member view is added with a manual view boundary.  
    **/  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_drf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    /* qq3123197280 */  
    static void do_it(void)  
    {     
        tag_t  
            view_to_add = NULL_TAG,  
            owning_part = NULL_TAG,  
            drawing = NULL_TAG,  
            drawing_part = NULL_TAG,  
            draw_view_tag = NULL_TAG;  
       char  
            dname[31] = { "TEST_DWG" },  
            vname[31] = { "" },  
            pname[ MAX_FSPEC_SIZE+1 ] = { "" };  
       double  
           dwg_ref_pt[2] = { 150., 150. };  
        UF_DRAW_info_t  
            draw_info = { UF_DRAW_METRIC_SIZE, UF_DRAW_A2, 1.0, UF_PART_METRIC,  
                UF_DRAW_FIRST_ANGLE_PROJECTION };  
        UF_DRAW_view_info_t   
            view_info;  
        UF_DRAW_boundary_type_t   
            boundary_type;  
        if( UF_PART_ask_display_part() == NULL_TAG ) return;  
        UF_DRAW_ask_current_drawing ( &drawing );  
        if( NULL_TAG != drawing ) return;  
        UF_VIEW_ask_work_view ( &view_to_add );  
        UF_CALL( UF_OBJ_ask_name( view_to_add, vname ));   
        UF_CALL( UF_OBJ_ask_owning_part( view_to_add, &owning_part ) );  
        UF_CALL( UF_PART_ask_part_name( owning_part, pname ));  
        UF_CALL( UF_PART_new( "a_new_part", UF_PART_METRIC, &drawing_part ));  
        UF_CALL( UF_DRAW_create_drawing( dname, &draw_info, &drawing ));  
        UF_DRAW_initialize_view_info( &view_info );  
        view_info.view_status = UF_DRAW_ACTIVE_VIEW;  
        view_info.anchor_point = NULL_TAG;  
        view_info.view_scale = 1.0;  
        view_info.use_ref_pt = FALSE;  
        view_info.clean_model_view = FALSE;  
        view_info.inherit_boundary = TRUE;  
        view_info.transfer_annotation = FALSE;  
        strcpy( view_info.model_name, pname );  
        view_info.arrangement_name[0] = '\0';  
        UF_DRAW_import_view( drawing, view_to_add, dwg_ref_pt,   
                                             &view_info, &draw_view_tag );  
        UF_CALL(UF_OBJ_ask_name( drawing, dname ));  
        UF_CALL(UF_DRF_update_views( dname, UF_DRF_UPDATE_ALL, NULL ));  
        UF_CALL(UF_DRAW_upd_out_of_date_views ( drawing ));  
        UF_CALL( UF_DRAW_ask_boundary_type(draw_view_tag, &boundary_type ));  
        switch(  boundary_type )  
        {  
            case UF_DRAW_BREAK_DETAIL_TYPE:  
                WRITE( "UF_DRAW_BREAK_DETAIL_TYPE\n" );  
                break;  
            case UF_DRAW_MANUAL_RECTANGLE_TYPE:   
                WRITE( "UF_DRAW_MANUAL_RECTANGLE_TYPE\n" );  
                break;  
            case UF_DRAW_AUTOMATIC_RECTANGLE_TYPE:  
                WRITE( "UF_DRAW_AUTOMATIC_RECTANGLE_TYPE\n" );  
                break;  
            case UF_DRAW_BOUND_BY_OBJECTS_TYPE:    
                WRITE( "UF_DRAW_BOUND_BY_OBJECTS_TYPE\n" );  
                break;  
        }  
        /* this is necessary to see the geometry in the imported view 里海译:这是为了在导入的视图中看到几何图形。 */  
        UF_CALL( UF_DISP_regenerate_display());  
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

> 这段代码是一个NX Open C++程序，主要功能是将当前工作视图导入到一个新建的绘图部分中。以下是代码的主要功能：
>
> 1. 包含必要的NX Open C++头文件，以便调用NX提供的API。
> 2. 定义了错误报告函数report_error，用于打印出API调用失败的错误信息。
> 3. 定义了宏WRITE，用于向列表窗口打印信息。
> 4. 定义了主函数do_it()，执行以下操作：检查当前是否打开了显示部件，如果没有则退出。获取当前工作视图的tag。获取工作视图所属部件的名称。创建一个新部件"a_new_part"。创建一个新绘图，名称为"TEST_DWG"。设置视图信息，包括视图状态、缩放比例等。将工作视图导入到新建的绘图中。更新视图并重新生成显示。
> 5. 检查当前是否打开了显示部件，如果没有则退出。
> 6. 获取当前工作视图的tag。
> 7. 获取工作视图所属部件的名称。
> 8. 创建一个新部件"a_new_part"。
> 9. 创建一个新绘图，名称为"TEST_DWG"。
> 10. 设置视图信息，包括视图状态、缩放比例等。
> 11. 将工作视图导入到新建的绘图中。
> 12. 更新视图并重新生成显示。
> 13. 定义了ufusr()函数，用于初始化NX Open，调用do_it()函数，然后终止NX Open。
> 14. 定义了ufusr_ask_unload()函数，返回立即卸载标志，表示该程序可以被立即卸载。
>
> 总体来说，这段代码的主要功能是获取当前工作视图，然后将其导入到一个新建的绘图部分中，以便在绘图中查看该视图的几何图形。
>
