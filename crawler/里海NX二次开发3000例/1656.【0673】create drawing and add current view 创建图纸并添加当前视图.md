### 【0673】create drawing and add current view 创建图纸并添加当前视图

#### 代码

```cpp
    /*HEAD CREATE_DRAWING_AND_ADD_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_view.h>  
    #include <uf_draw.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的函数。它用于将日志信息打印到系统日志。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            drawing,  
            member_view,  
            original_view;  
        double  
            center[3],  
            middle[2] = { 11, 8.5 },  
            scale;  
        UF_DRAW_info_t  
            drawing_info = { UF_DRAW_ENGLISH_SIZE, UF_DRAW_C, 1.0, UF_PART_ENGLISH,  
                UF_DRAW_THIRD_ANGLE_PROJECTION };  
        UF_DRAW_view_info_t  
            view_info;  
        set_undo_mark("Create Drawing");  
    /*  UF_DRAW_initialize_view_info sets these defaults:  
        view_info->view_status = UF_DRAW_ACTIVE_VIEW;  
        view_info->anchor_point = NULL_TAG;  
        view_info->view_scale = 1.0;  
        view_info->use_ref_pt = FALSE;  
        view_info->transfer_annotation = TRUE;  
        view_info->inherit_boundary = FALSE;  
        view_info->model_name[0] = '\0';  
        view_info->arrangement_name[0] = '\0';  
    */  
        UF_DRAW_initialize_view_info(&view_info);  
        view_info.use_ref_pt = TRUE;  
        view_info.transfer_annotation = FALSE;  
        UF_CALL(UF_VIEW_ask_work_view(&original_view));  
        WRITENZ(uc6432("", 1));               /*  Fit view 译:Fit view 翻译为“适合视图” */  
        WRITENZ(uc6430("", center, &scale));  /*  Read current center point 译:翻译 Read current center point */  
        WRITENZ(uc6443("", center, 0.0));     /*  Set it as reference point 译:设为参考点 */  
        WRITENZ(uc6450("", "", 0, 0));        /*  Save these changes 译:保存这些更改 */  
        UF_CALL(UF_DRAW_create_drawing("SH1", &drawing_info, &drawing));  
        UF_CALL(UF_DRAW_import_view(drawing, original_view, middle, &view_info,  
            &member_view));  
        // This seems to be necessary to see wireframe geometry in NX9+  
        UF_CALL(UF_DRAW_update_one_view(drawing, member_view));  
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
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
                    build_copy_partname(new_fspec);  
                    if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                        printf("\tsaved as %s\n", new_fspec);  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发示例，其主要功能包括：
>
> 1. 初始化NX环境，并设置撤销标记点。
> 2. 获取当前工作视图，并读取其中心点，将其设为参考点。
> 3. 创建一个新的图纸，并根据当前视图创建图纸中的第一个视图。
> 4. 更新视图以显示线框几何。
> 5. 遍历所有打开的部件，为每个部件创建图纸并保存为新的文件。
> 6. 在主函数中，遍历所有加载的部件，并为每个部件执行上述操作。
> 7. 记录处理的部件数量。
> 8. 在最后，清理环境并终止NX。
>
> 总体来说，这段代码通过二次开发实现了自动化创建图纸和视图的功能，并支持撤销操作。
>
