### 【1886】report 2d bounding box of wireframe objects in current drawing 报告当前图纸中线框对象的2D边界框

#### 代码

```cpp
    /*HEAD REPORT_2D_BOUNDING_BOX_OF_WIREFRAME_OBJECTS_IN_CURRENT_DRAWING CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_draw.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE2F(X)  (write_double_array_to_listing_window(#X, X, 1, 2))  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    #define WRITE9F(X)  (write_double_array_to_listing_window(#X, X, 3, 3))  
    #define WRITE16F(X) (write_double_array_to_listing_window(#X, X, 4, 4))  
    #define WRITEnF(n,X)(write_double_array_to_listing_window(#X, X, 1, n))  
    #define WRITEnF4(X,n) (write_double_array_to_listing_window(#X, X, n, 4))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            type = 0,  
            subtype = 0,  
            num_views = 0;  
        tag_t  
            drawing_view = NULL_TAG,  
            part = NULL_TAG,  
            drawing = NULL_TAG,  
            entity = NULL_TAG,  
            *views = NULL;  
        double  
            box[6],  
            map_pt[2],  
            dwg_min[2] = { 1000000., 1000000. },  
            dwg_max[2] =  { -1000000., -1000000. },   
            min[3] = { 0., 0., 0. },  
            max[3] =  { 0., 0., 0. };  
        part = UF_PART_ask_display_part();  
        if( NULL_TAG == part ) return;  
        UF_CALL( UF_DRAW_ask_current_drawing ( &drawing ));  
        if( NULL_TAG == drawing ) return;  
        UF_CALL( UF_DRAW_ask_view_of_drawing( drawing, &drawing_view ));  
        /* cycle views on the drawing 里海译:在图纸上的循环视图 */  
        UF_CALL( UF_DRAW_ask_views( drawing, &num_views, &views ));  
        for( inx = 0; inx < num_views; inx++ )  
        {  
            while ( (!UF_VIEW_cycle_objects(views[inx], UF_VIEW_VISIBLE_OBJECTS, &entity )) &&  
                    entity != NULL_TAG )  
            {  
                UF_CALL( UF_OBJ_ask_type_and_subtype( entity, &type, &subtype ));  
                switch ( type )  
                {  
                    case  UF_line_type:    
                    case  UF_circle_type:    
                    case  UF_conic_type:    
                    case  UF_spline_type:    
                                                       /* check bounding box 里海译:检查边界框 */  
                        UF_CALL(UF_MODL_ask_bounding_box(entity, box));  
                        min[0] = box[0];  
                        min[1] = box[1];  
                        min[2] = box[2];  
                        max[0] = box[3];  
                        max[1] = box[4];  
                        max[2] = box[5];  
                        UF_VIEW_map_model_to_drawing( views[inx], min, map_pt );  
                        if( map_pt[0] < dwg_min[0] ) dwg_min[0] = map_pt[0];  
                        if( map_pt[1] < dwg_min[1] ) dwg_min[1] = map_pt[1];  
                        UF_VIEW_map_model_to_drawing( views[inx], max, map_pt );  
                        if( map_pt[0] > dwg_max[0] ) dwg_max[0] = map_pt[0];  
                        if( map_pt[1] > dwg_max[1] ) dwg_max[1] = map_pt[1];  
                    break;  
                    default:  continue;  
                }  
            }  
        }  
        if( num_views > 0 ) UF_free( views );  
        WRITE( "\nBounding Box in Drawing Coordinates:\n" );  
        WRITE2F( dwg_min );  
        WRITE2F( dwg_max );  
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

> 这段代码是一个NX的二次开发代码，其主要功能是获取当前图纸中所有线框对象的2D边界框。代码的关键点如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于输出错误代码和错误信息。
> 2. 数组写入函数：定义了多个数组写入函数，用于将浮点数数组写入列表窗口。
> 3. 获取当前零件：通过UF_PART_ask_display_part获取当前显示的零件。
> 4. 获取当前图纸和视图：通过UF_DRAW_ask_current_drawing和UF_DRAW_ask_view_of_drawing获取当前图纸和视图。
> 5. 遍历所有视图中的对象：通过UF_DRAW_ask_views和UF_VIEW_cycle_objects遍历所有视图中的对象。
> 6. 获取对象类型和子类型：通过UF_OBJ_ask_type_and_subtype获取对象类型和子类型。
> 7. 获取边界框：对于线、圆、圆锥和样条线对象，通过UF_MODL_ask_bounding_box获取边界框，并将其映射到图纸坐标系。
> 8. 输出边界框信息：将所有对象的边界框合并，得到整个图纸的边界框，并输出。
> 9. 清理视图数组：遍历完成后，释放视图数组。
> 10. 主函数：定义了ufusr主函数，初始化NX环境，调用do_it函数，然后终止NX环境。
> 11. 卸载函数：定义了卸载函数，返回立即卸载标志。
>
> 总体来说，这段代码通过遍历图纸中的视图和对象，获取线框对象的边界框，并最终输出整个图纸的2D边界框。
>
