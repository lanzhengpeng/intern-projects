### 【3137】report all drafting aid text info in external mode 在UG外部模式下报告所有草图辅助文字信息的功能

#### 代码

```cpp
    /*HEAD REPORT_ALL_DRAFTING_AID_TEXT_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_drf.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog 是 V18 版本新增的。 */  
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
    static tag_t ask_next_drafting_aid(tag_t part, tag_t draft)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_drafting_entity_type, &draft));  
        return (draft);  
    }  
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 里海译:头文件中的非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 里海译:如果找不到，请只回答翻译结果，不要添加任何无关内容。 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        if (status == UF_OBJ_DELETED)  
        {  
            sprintf(msg, ", status: %d\n", status);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            UF_CALL(UF_UI_write_listing_window(", occurrence"));  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                UF_CALL(UF_UI_write_listing_window(" (from "));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_UI_write_listing_window(")"));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    static void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
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
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_drafting_aid_text_info(tag_t draft_aid)  
    {  
        int  
            ii,  
            jj,  
            n_strings;  
        UF_DRF_draft_aid_text_info_t  
            *text_info = NULL;  
        UF_CALL(UF_DRF_ask_draft_aid_text_info(draft_aid, &n_strings, &text_info));  
        WRITE_D(n_strings);  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(text_info[ii].text_type);  
            WRITE_D(text_info[ii].text_font);  
            WRITE_F(text_info[ii].size);  
            WRITE_F(text_info[ii].angle);  
            WRITE3F(text_info[ii].origin);  
            display_temporary_point(text_info[ii].origin);  
            WRITE_F(text_info[ii].length);  
            WRITE_F(text_info[ii].height);  
            WRITE_F(text_info[ii].distance);  
            WRITE_F(text_info[ii].aspect_ratio);  
            WRITE_F(text_info[ii].gap);  
            WRITE_F(text_info[ii].line_spacing);  
            WRITE_D(text_info[ii].num_lines);  
            for (jj=0; jj<text_info[ii].num_lines; jj++)  
            {  
                WRITE_D(jj);  
                WRITE_D(text_info[ii].text[jj].num_chars);  
                WRITE_S(text_info[ii].text[jj].string);  
                WRITE_D(text_info[ii].text[jj].num_ints);  
                WRITE_D(text_info[ii].text[jj].full_num_chars);  
                WRITE_S(text_info[ii].text[jj].full_string);  
            }  
        }  
        if( text_info != NULL )  
            UF_CALL(UF_DRF_free_text(n_strings, &text_info));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            draft_aid = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((draft_aid = ask_next_drafting_aid(part, draft_aid)) != NULL_TAG)  
        {  
            RX(draft_aid);  
            report_drafting_aid_text_info(draft_aid);  
        }  
    }  
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
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    do_it();  
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

> 这段代码是一个NX Open C++应用程序，其主要功能是遍历当前显示部件中的所有绘图辅助对象，并报告每个绘图辅助对象的文本信息。
>
> 主要步骤包括：
>
> 1. 初始化NX Open环境。
> 2. 获取当前显示的部件。
> 3. 遍历部件中的所有绘图辅助对象，并获取每个绘图辅助对象的文本信息，包括文本类型、字体、大小、角度、原点、长度、高度、间距、行数、每行文本等。
> 4. 将文本信息输出到列表窗口。
> 5. 关闭所有部件并终止NX Open环境。
> 6. 主函数中，还可以通过命令行参数指定部件路径，并处理指定的部件。
> 7. 提供了多个辅助函数，用于报告对象的简单信息、写入字符串、写入浮点数、写入整数等。
> 8. 使用了宏定义简化代码。
> 9. 通过错误处理函数统一处理函数调用失败的情况。
>
> 总体来说，这段代码提供了获取和报告NX部件中绘图辅助文本信息的功能，并展示了NX Open C++应用程序的基本结构和常用技巧。
>
