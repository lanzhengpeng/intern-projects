### 【1312】fix prev12 holes 修复V12.0之前版本通过UFUNC创建的特征孔洞

#### 代码

```cpp
    /* THIS PROGRAM WILL FIX THE PROBLEM OF NOT BEING ABLE TO UPDATE  
       FEATURE LISTS DUE TO HOLES CREATED PRIOR TO V12.0 VIA UFUNC. PROGRAM  
       WILL SIMPLY REATTACH THE FACE TO THE THE FACE IT'S LINKED TO.  
       CAN BE LINKED FOR INTERNAL OR EXTERNAL EXECUTION             */  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdio.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_defs.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
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
    static void do_it(void)  
    {  
        int  
            ii,  
            nfeat;  
        tag_t  
            body = NULL_TAG,  
            dir_ref,  
            item,  
            part_tag = UF_PART_ask_display_part(),  
            place_face,  
            tool_face;  
        logical  
            is_ref_hor;  
        char  
            *type;  
        uf_list_p_t  
            feat_list;  
        double  
            feat_location[3];  
        while((body = ask_next_body(part_tag, body)) != 0)  
        {  
            UF_CALL(UF_MODL_ask_body_feats( body, &feat_list ));  
            UF_CALL(UF_MODL_ask_list_count( feat_list, &nfeat ));  
            for( ii = nfeat - 1; ii > -1; ii-- )  
            {  
                UF_CALL(UF_MODL_ask_list_item( feat_list, ii, &item ));  
                UF_CALL(UF_MODL_ask_feat_type( item, &type ));  
                if(!(strcmp(type,"SIMPLE HOLE")) ||  
                   !(strcmp(type, "CBORE_HOLE")) ||  
                   !(strcmp(type, "CSUNK_HOLE")))  
                {  
                    /* get face that the feat was attached to 里海译:这个短语"get face that the feat was attached to"的翻译是：“获得与这一成就相关的面子”。 */  
                    UF_CALL(UF_MODL_ask_link_faces(item, &place_face,  
                              &tool_face, &dir_ref, &is_ref_hor));  
                    /* get it's original location 里海译:获取它的原始位置 */  
                    UF_CALL(UF_MODL_ask_feat_location(item, feat_location));  
                    /* reattaches it to the original face 里海译:reattaches it to the original face的翻译为“重新把它贴回原来的脸上”。 */  
                    UF_CALL(UF_MODL_reattach_target_face(item, place_face,  
                            feat_location, 0, 0));  
                }  
                UF_free(type);  
            }  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        int  
            ii;  
        char  
            msg[133];  
        if (status->n_parts > 0)  
        {  
            ECHO("Load notes:\n");  
            for (ii = 0; ii < status->n_parts; ii++)  
            {  
                UF_get_fail_message(status->statuses[ii], msg);  
                ECHO("  %s - %s\n", status->file_names[ii], msg);  
            }  
            ECHO("\n");  
            UF_free(status->statuses);  
            UF_free_string_array(status->n_parts, status->file_names);  
        }  
    }  
    int main( int argc, char *argv[] )  
    {  
        if (UF_CALL(UF_initialize())) return 1;  
        if (argc > 1)  
        {  
            int  
                cnt = 0,  
                ii;  
            logical  
                new_flag  = FALSE,  
                save_flag = FALSE;  
            tag_t  
                part;  
            UF_PART_load_status_t  
                status;  
            for (ii=1; ii<argc; ii++)  
            {  
                if (!strcmp(argv[ii], "-save")) save_flag = TRUE;  
                else if (!strcmp(argv[ii], "-new")) new_flag = TRUE;  
                else  
                {  
                    printf("%d.  %s\n", ++cnt, argv[ii]);  
                    if (new_flag)  
                    {  
                        if (!UF_CALL(UF_PART_new(argv[ii], UF_PART_ENGLISH,  
                            &part)))  
                        {  
                            do_it();  
                            if (save_flag)  UF_CALL(UF_PART_save());  
                            UF_PART_close_all();  
                        }  
                    }  
                    else  
                    {  
                        if (UF_CALL(UF_PART_open(argv[ii], &part, &status))  
                            || status.n_parts > 0)  
                            report_load_status(&status);  
                        else  
                        {  
                            do_it();  
                            if (save_flag)  UF_CALL(UF_PART_save());  
                            UF_PART_close_all();  
                        }  
                    }  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
        }  
        else do_it();  
        UF_CALL(UF_terminate());  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码旨在修复一个与特征列表更新相关的问题，具体来说，是修复由于在V12.0之前通过UFUNC创建孔洞而导致的特征列表无法更新的问题。代码的核心功能是重新附着特征到其原始相关的面。
>
> 主要步骤包括：
>
> 1. 初始化NX环境
> 2. 遍历零件中的实体，获取每个实体的特征列表
> 3. 遍历特征列表中的每个特征，判断特征类型是否为孔特征（如SIMPLE HOLE、CBORE_HOLE、CSUNK_HOLE）
> 4. 获取孔特征附着到的原始面
> 5. 获取孔特征的原始位置
> 6. 重新将孔特征附着到原始面上
> 7. 重复上述过程，直到所有孔特征都被重新附着
> 8. 终止NX环境
>
> 该代码可以通过参数指定要处理的零件文件，并且可以保存修改后的零件文件。如果无参数，则默认处理当前打开的零件。该代码使用NX提供的UFUNC接口实现，具有较好的通用性和可移植性。
>
