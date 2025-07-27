### 【0637】create bmp of all drawing sheets 为所有图纸生成BMP图像

#### 代码

```cpp
    /*HEAD CREATE_BMP_OF_ALL_DRAWING_SHEETS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_draw.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：请注意，UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t ask_next_drawing(tag_t part, tag_t drawing)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drawing_type, &drawing));  
        return (drawing);  
    }  
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
        build_temp_filespec(temp_fspec, 0);  
        while ((drawing = ask_next_drawing(part, drawing)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
            sprintf(export_fspec, "%s-%s.bmp", temp_fspec, draw_name);  
            UF_CALL(UF_DRAW_open_drawing(drawing));  
            UF_CALL(UF_DISP_create_image(export_fspec, UF_DISP_BMP,  
            UF_DISP_ORIGINAL));  
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

> 这段NX二次开发代码的主要功能是创建当前显示部件的所有图纸的位图文件，并将它们保存到临时目录下。
>
> 主要步骤如下：
>
> 1. 定义了一个错误报告函数report_error，用于打印函数调用失败时的错误信息。
> 2. 定义了一个辅助函数ask_next_drawing，用于遍历部件中的所有图纸。
> 3. 定义了一个辅助函数build_temp_filespec，用于构建临时文件路径。
> 4. 在do_it函数中，首先获取当前显示的部件，然后遍历该部件的所有图纸。
> 5. 对于每个图纸，获取其名称，构建导出位图的文件路径，然后打开图纸，并调用UF_DISP_create_image函数创建位图文件。
> 6. 在ufusr函数中，初始化NX，调用do_it函数执行创建位图文件的任务，然后终止NX。
> 7. 在ufusr_ask_unload函数中，返回立即卸载标识，表示该二次开发程序不需要在NX中保持加载状态。
>
> 总的来说，这段代码实现了从NX部件中提取所有图纸并生成位图文件的功能，为图纸的进一步处理提供了基础。
>
