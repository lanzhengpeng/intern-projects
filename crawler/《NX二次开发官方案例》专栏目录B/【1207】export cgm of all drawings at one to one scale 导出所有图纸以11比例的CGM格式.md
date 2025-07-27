### 【1207】export cgm of all drawings at one to one scale 导出所有图纸以11比例的CGM格式

#### 代码

```cpp
    /*HEAD EXPORT_CGM_OF_ALL_DRAWINGS_AT_ONE_TO_ONE_SCALE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_cgm.h>  
    #include <uf_ui.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_std.h>  
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
    /* qq3123197280 */  
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
    /* qq3123197280 */  
    static void build_temp_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(tmp_dir, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            export_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            temp_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_CGM_export_options_t   
            export_options;  
        build_temp_filespec(temp_fspec, 0);  
        UF_CALL(UF_CGM_ask_session_export_options( &export_options ));  
        export_options.size.mode = UF_CGM_SIZE_BY_SCALE;  
        export_options.size.scale = 1.0;  
        UF_CALL(UF_CGM_set_session_export_options( &export_options ));  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
            sprintf(export_fspec, "%s_%s.cgm", temp_fspec, draw_name);  
            WRITE( "Creating CGM FILE: " );  
            WRITE( export_fspec );  
            WRITE( "\n" );  
            UF_CALL(UF_CGM_export_cgm( drawing, &export_options, export_fspec));  
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

> 这段代码是NX二次开发代码，主要功能是导出当前显示部件中的所有绘图到CGM文件，并且以1:1的比例导出。具体来说：
>
> 1. 定义了错误报告函数report_error，用于打印错误信息。
> 2. 定义了函数ask_next_drawing，用于循环遍历部件中的所有绘图。
> 3. 定义了函数build_temp_filespec，用于构建临时文件路径。
> 4. 在do_it函数中，首先获取当前显示的部件，然后循环遍历部件中的所有绘图，并为每个绘图构建一个临时CGM文件路径。
> 5. 设置CGM导出选项为1:1比例。
> 6. 对每个绘图调用UF_CGM_export_cgm函数导出到CGM文件，并打印文件路径。
> 7. ufusr函数中调用do_it函数执行导出操作。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 总体来说，这段代码实现了遍历部件中的所有绘图，并以1:1比例导出到CGM文件的功能。
>
