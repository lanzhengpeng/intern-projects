### 【0074】add part to assembly 在装配中添加零件

#### 代码

```cpp
    /*HEAD ADD_PART_TO_ASSEMBLY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_cfi.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，V18版本新增了一个名为UF_print_syslog的函数。 */  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        if (status->n_parts > 0)  
        {  
            UF_UI_open_listing_window();  
            UF_UI_write_listing_window("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_UI_write_listing_window("  ");  
                UF_UI_write_listing_window(status->file_names[ii]);  
                UF_UI_write_listing_window(" - ");  
                UF_get_fail_message(status->statuses[ii], msg);  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window("\n");  
            }  
            UF_UI_write_listing_window("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            instance,  
            part = UF_PART_ask_display_part();  
        double  
            origin[3] = { 0, 0, 0 },  
            matrix[6] = { 1,0,0, 0,1,0 };  
        UF_PART_load_status_t  
            error_status;  
        UF_CALL(UF_ASSEM_add_part_to_assembly(part, "parts/a.prt",  
            NULL, NULL, origin, matrix, 0, &instance, &error_status));  
        report_load_status(&error_status);  
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
                report_load_status(&status);  
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

> 这段代码是用于NX的二次开发，主要功能包括：
>
> 1. 添加部件到装配体：使用UF_ASSEM_add_part_to_assembly函数将指定部件添加到装配体中。
> 2. 错误报告：通过自定义的report_error函数来报告函数调用错误。
> 3. 部件加载状态报告：通过自定义的report_load_status函数来报告部件加载状态。
> 4. 处理多个部件：主函数中循环处理传入的多个部件文件，打开每个部件，将其添加到装配体中，然后保存为新的文件。
> 5. 函数封装：将主要逻辑封装在do_it函数中，主函数负责初始化和循环处理。
> 6. 用户界面：使用UF_UI函数显示加载状态和错误信息。
> 7. 辅助函数：提供build_copy_partname函数生成新部件文件名。
>
> 总体而言，这段代码通过NX提供的API实现了添加部件到装配体的功能，并包含了错误处理、用户界面提示等功能，适合进行二次开发。
>
