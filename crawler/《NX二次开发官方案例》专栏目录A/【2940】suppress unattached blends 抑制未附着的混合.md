### 【2940】suppress unattached blends 抑制未附着的混合

#### 代码

```cpp
    /*HEAD SUPPRESS_UNATTACHED_BLENDS CCC UFUN */  
    /*  
       User Function program to suppress blend and chamfer features which are  
       not attached to the model at part load time.  
       The name of the part to check is input to the program.  For example:  
       suppress_unattached_blends partname.prt  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static void do_it(tag_t part)  
    {  
        int  
            n,  
            is_suppressed;  
        char  
            *feat_name;  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            face_list,  
            suppress_list;  
        UF_CALL(UF_MODL_create_list(&suppress_list));  
        while  (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &feat_name));  
            if ( (!strcmp(feat_name,"BLEND")) ||  
                 (!strcmp(feat_name,"CHAMFER")) )  
            {  
                UF_CALL(UF_MODL_ask_feat_faces(feat, &face_list));  
                if (face_list->eid == NULL_TAG)  
                {  
                    UF_free(feat_name);  
                    UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
                    UF_CALL(UF_MODL_ask_suppress_feature(feat, &is_suppressed));  
                    if (is_suppressed)  
                        printf("\tSuppressed feature = %s\n", feat_name);  
                    else  
                    {  
                        printf("\tUnattached feature = %s\n", feat_name);  
                        UF_CALL(UF_MODL_put_list_item(suppress_list, feat));  
                    }  
                }  
                UF_CALL(UF_MODL_delete_list(&face_list));  
            }  
            UF_free(feat_name);  
        }  
        UF_CALL(UF_MODL_ask_list_count(suppress_list, &n));  
        if ((n > 0) && !UF_CALL(UF_MODL_suppress_feature(suppress_list)))  
                printf("\t%d unattached features suppressed.\n", n);  
        UF_CALL(UF_MODL_delete_list(&suppress_list));  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        tag_t  
            part;  
        if (!UF_CALL(UF_initialize()))  
        {  
            part = UF_PART_ask_display_part();  
            do_it(part);  
            UF_CALL(UF_terminate());  
        }  
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
    int main(int argc, char *argv[])  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            int  
                cnt = 0,  
                ii;  
            tag_t  
                part;  
            UF_PART_load_status_t  
                status;  
            UF_ASSEM_options_t  
                assy_options;  
            /* Do not load components if it is Assembly part 里海译:不要加载组件，如果它是装配的一部分。 */  
            UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
            assy_options.load_components = UF_ASSEM_dont_load_components;  
            UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
            for (ii=1; ii<argc; ii++)  
            {  
                printf("%d.  %s\n", ++cnt, argv[ii]);  
                if (UF_CALL(UF_PART_open(argv[ii], &part, &status)))  
                    check_load_status(&status);  
                else  
                {  
                    do_it(part);  
                    UF_CALL(UF_PART_save());  
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

> 这段代码是一个用于NX的二次开发程序，其主要功能是在加载模型时自动抑制未附着到模型上的倒圆角和倒角特征。
>
> 代码的关键要点如下：
>
> 1. 包含了必要的NX API头文件。
> 2. 定义了一个报告错误函数report_error，用于输出错误信息。
> 3. 定义了一个处理函数do_it，用于遍历模型中的所有特征，检查其是否为未附着到模型上的倒圆角或倒角特征，并创建一个列表来保存这些特征。
> 4. 主函数中，首先初始化NX API，然后遍历传入的零件文件列表，打开每个文件，调用do_it函数处理，保存修改并关闭文件。
> 5. 在处理每个文件时，先不加载组件，然后调用UF_PART_open打开文件，获取零件tag，调用do_it函数处理，保存修改，关闭所有文件。
> 6. do_it函数中，遍历零件中的所有特征，检查其是否为倒圆角或倒角特征，获取其附着面列表，如果面列表为空，则将特征添加到待抑制列表中，最后统一抑制这些特征。
> 7. 使用了NX提供的对象循环、特征查询、列表管理等API来实现以上功能。
>
> 总的来说，这段代码通过NX提供的API，实现了在加载模型时自动抑制未附着到模型上的倒圆角和倒角特征的功能，提高了模型加载的效率。
>
