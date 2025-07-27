### 【1908】report all drawing sizes 报告所有绘图尺寸

#### 代码

```cpp
    /*HEAD REPORT_ALL_DRAWING_SIZES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_draw.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，我翻译如下：

请注意，UF_print_syslog是在V18版本中新增的。 */  
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
    static void report_drawing_size(tag_t drawing)  
    {  
        char  
            draw_name[MAX_ENTITY_NAME_SIZE+1],  
            msg[MAX_LINE_SIZE+1],  
            *unit_names[2] = { "mm", "inches" };  
        UF_DRAW_info_t  
            info;  
        UF_UI_open_listing_window();  
        UF_CALL(UF_OBJ_ask_name(drawing, draw_name));  
        UF_UI_write_listing_window(draw_name);  
        UF_CALL(UF_DRAW_ask_drawing_info(drawing, &info));  
        switch (info.size_state)  
        {  
            case UF_DRAW_METRIC_SIZE:  
                switch (info.size.metric_size_code)  
                {  
                    case UF_DRAW_A0:  
                        UF_UI_write_listing_window(" - A0 (841 x 1189 mm)\n");  
                        break;  
                    case UF_DRAW_A1:  
                        UF_UI_write_listing_window(" - A1 (594 x 841 mm)\n");  
                        break;  
                    case UF_DRAW_A2:  
                        UF_UI_write_listing_window(" - A2 (420 x 594 mm)\n");  
                        break;  
                    case UF_DRAW_A3:  
                        UF_UI_write_listing_window(" - A3 (297 x 420 mm)\n");  
                        break;  
                    case UF_DRAW_A4:  
                        UF_UI_write_listing_window(" - A4 (210 x 297 mm)\n");  
                        break;  
                    default:  
                        UF_UI_write_listing_window(" - Unknown metric size\n");  
                        break;  
                }  
                break;  
            case UF_DRAW_ENGLISH_SIZE:  
                switch (info.size.english_size_code)  
                {  
                    case UF_DRAW_A:  
                        UF_UI_write_listing_window(" - A (8.5 x 11 inches)\n");  
                        break;  
                    case UF_DRAW_B:  
                        UF_UI_write_listing_window(" - B (11 x 17 inches)\n");  
                        break;  
                    case UF_DRAW_C:  
                        UF_UI_write_listing_window(" - C (17 x 22 inches)\n");  
                        break;  
                    case UF_DRAW_D:  
                        UF_UI_write_listing_window(" - D (22 x 34 inches)\n");  
                        break;  
                    case UF_DRAW_E:  
                        UF_UI_write_listing_window(" - E (34 x 44 inches)\n");  
                        break;  
                    case UF_DRAW_F:  
                        UF_UI_write_listing_window(" - F (28 x 40 inches)\n");  
                        break;  
                    case UF_DRAW_H:  
                        UF_UI_write_listing_window(" - H (28 x 44 inches)\n");  
                        break;  
                    case UF_DRAW_J:  
                        UF_UI_write_listing_window(" - J (34 x 55 inches)\n");  
                        break;  
                    default:  
                        UF_UI_write_listing_window(" - Unknown english size\n");  
                        break;  
                }  
                break;  
            case UF_DRAW_CUSTOM_SIZE:  
                sprintf(msg, " - Custom size (%.1f x %.1f %s)\n",  
                    info.size.custom_size[0], info.size.custom_size[1],  
                    unit_names[info.units - 1]);  
                UF_UI_write_listing_window(msg);  
                break;  
            default:  
                UF_UI_write_listing_window(" - Unknown size\n");  
                break;  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_drawings;  
        tag_t  
            *drawings;  
        UF_CALL(UF_DRAW_ask_drawings(&n_drawings, &drawings));  
        for (ii = 0; ii < n_drawings; ii++) report_drawing_size(drawings[ii]);  
        if (n_drawings > 0) UF_free(drawings);  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX Open C++应用程序，其主要功能是获取NX图纸中的图纸尺寸信息，并进行报告。下面是代码的主要功能和流程：
>
> 1. 错误报告宏：使用UF_CALL宏来捕获函数调用错误，并报告错误信息。
> 2. 获取图纸尺寸信息：report_drawing_size函数通过UF_DRAW_ask_drawing_info获取图纸的尺寸信息，包括A0A4、AH、自定义尺寸等，并格式化输出。
> 3. 获取所有图纸对象：do_it函数通过UF_DRAW_ask_drawings获取当前打开的所有图纸对象，并遍历调用report_drawing_size报告每个图纸的尺寸信息。
> 4. UFusr函数：NX Open提供的接口函数，用于初始化和关闭NX Open。
> 5. 主函数：主函数中首先初始化NX Open，然后调用uc4624获取命令行参数指定的所有零件，打开每个零件，并调用do_it报告零件中所有图纸的尺寸信息。最后关闭所有零件并终止NX Open。
> 6. 辅助函数：包括获取失败信息、检查零件加载状态等辅助函数。
>
> 综上所述，该代码的主要功能是获取NX图纸的尺寸信息并进行报告，适用于NX二次开发环境。
>
