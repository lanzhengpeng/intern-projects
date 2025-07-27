### 【1671】name bad faces 这是一个用于识别和标记NX模型中坏面(face)的UF函数

#### 代码

```cpp
    /*HEAD NAME_BAD_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18中新增的，仅回答译文，不要赘述。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void extract_faces(tag_t body)  
    {  
        int  
            face_count,  
            ii;  
        tag_t  
            face,  
            sheet;  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces( body, &face_list));  
        UF_CALL(UF_MODL_ask_list_count(face_list, &face_count));  
        printf("Body %d contains %d faces.\n", body, face_count);  
        for (ii = 0; ii < face_count; ii++)  
        {  
            UF_CALL(UF_MODL_ask_list_item(face_list, ii, &face));  
            printf("  Face %d  = %d ...\n", ii + 1, face);  
            if (!UF_CALL(UF_MODL_extract_face(face, 2, &sheet)))  
                UF_CALL(UF_OBJ_delete_object(sheet));  
            else  
                UF_CALL(UF_OBJ_set_name(face, "BAD"));  
        }  
        UF_CALL( UF_MODL_delete_list( &face_list));  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            extract_faces(body);  
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
    void main( int argc, char *argv[] )  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            if (argc > 1)  
            {  
                int  
                    cnt = 0,  
                    ii;  
                logical  
                    save_flag = FALSE;  
                tag_t  
                    part;  
                UF_PART_load_status_t  
                    status;  
                for (ii=1; ii<argc; ii++)  
                {  
                    if (!strcmp(argv[ii], "-save")) save_flag = TRUE;  
                    else  
                    {  
                        printf("%d.  %s\n", ++cnt, argv[ii]);  
                        if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                            || status.n_parts > 0)  
                            check_load_status(&status);  
                        else  
                        {  
                            do_it();  
                            if (save_flag)  UF_CALL(UF_PART_save());  
                            UF_PART_close_all();  
                        }  
                    }  
                }  
                printf("\nProcessed %d parts.\n", cnt);  
            }  
            else do_it();  
            UF_CALL(UF_terminate());  
        }  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能如下：
>
> 1. 提取体中的面并检查是否可以成功提取：代码首先遍历指定部件中的所有体，并提取每个体的所有面。对于每个面，尝试提取成曲面片，如果提取成功则删除曲面片，如果提取失败则将该面重命名为"BAD"。
> 2. 错误处理：在执行NX API调用时，如果发生错误，会记录错误代码和行号，并在窗口中输出错误信息。
> 3. 命令行参数处理：可以通过命令行参数指定要处理的NX部件文件，并可以添加"-save"参数，在处理完成后保存部件。如果没有指定文件，则处理当前打开的部件。
> 4. 加载部件状态检查：在加载部件时，会检查加载状态，如果有错误，会输出错误信息。
> 5. 模块化设计：代码采用模块化设计，将不同功能封装到不同的函数中，提高了代码的复用性和可维护性。
> 6. 函数和变量命名规范：代码遵循了良好的函数和变量命名规范，提高了代码的可读性。
> 7. API调用封装：使用宏UF_CALL封装了NX API调用，便于统一处理错误。
> 8. 日志输出：提供了日志输出功能，可以记录错误信息到系统日志或窗口。
> 9. 主函数处理：在主函数中，根据命令行参数调用不同的处理函数，并最终结束NX会话。
>
> 总的来说，这段代码实现了对NX部件中面的提取和检查功能，具有较好的模块化设计和错误处理机制。
>
