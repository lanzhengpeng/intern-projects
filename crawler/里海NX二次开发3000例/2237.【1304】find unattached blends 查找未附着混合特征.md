### 【1304】find unattached blends 查找未附着混合特征

#### 代码

```cpp
    /*HEAD FIND_UNATTACHED_BLENDS CCC UFUN */  
    /*  
       User Function program to output blend and chamfer features which are  
       not attached to the model at part load time.  
       The name of the part to check is input to the program.  For example:  
        find_unattached_blends partname.prt  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_assem.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的。 */  
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
        logical  
            any = FALSE;  
        int  
            is_suppressed ;  
        char  
            *feat_name ;  
        tag_t  
            feat = NULL_TAG;  
        uf_list_p_t  
            face_list ;  
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
                    any = TRUE;  
                    UF_CALL(UF_MODL_ask_feat_name(feat, &feat_name));  
                    UF_CALL(UF_MODL_ask_suppress_feature(feat, &is_suppressed));  
                    if (is_suppressed)  
                        printf("\tSuppressed feature = %s\n", feat_name) ;  
                    else  
                        printf("\tUnattached feature = %s\n", feat_name) ;  
                }  
                UF_CALL(UF_MODL_delete_list(&face_list));  
            }  
        }  
        if (!any) printf("\tNo unattached features found.\n");  
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
    static void report_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("\t%s - %s\n", status->file_names[ii], msg);  
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
        tag_t  
            part;  
        UF_PART_load_status_t  
            status;  
        UF_ASSEM_options_t  
            assy_options;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            if (uc4621(part_name) == 1)  
            {  
                printf("%s\n", part_name);  
                /* Do not load components if it is Assembly part 里海译:不要加载组件，如果是装配部件。 */  
                UF_CALL(UF_ASSEM_ask_assem_options(&assy_options));  
                assy_options.load_components = UF_ASSEM_dont_load_components;  
                UF_CALL(UF_ASSEM_set_assem_options(&assy_options));  
                if (UF_CALL(UF_PART_open(part_name, &part, &status)) ||  
                    status.n_parts > 0)  
                    report_load_status(&status);  
                else  
                {  
                    do_it(part);  
                    UF_PART_close_all(); /* Cleans up undo files 里海译:清理撤销文件 */  
                }  
            }  
            else  
            {  
                printf("\nThis program requires a part file name as input.\n");  
                printf("The syntax for this program is as follows:\n\n");  
                printf("\t%s <part_name>\n", argv[0]);  
            }  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段代码是一个NX二次开发程序，旨在检查NX部件中未附着到模型的混合和倒角特征。
>
> 主要功能包括：
>
> 1. 初始化NX环境。
> 2. 获取要检查的部件名称，如果是装配体，则不加载组件。
> 3. 打开部件，获取部件tag。
> 4. 遍历部件中的所有特征，检查特征类型是否为混合(BLEND)或倒角(CHAMFER)。
> 5. 对于混合和倒角特征，获取特征关联的面列表。
> 6. 如果特征的面列表为空，表示特征未附着到模型，输出特征名称。
> 7. 清理环境，关闭部件。
> 8. 输出使用说明。
> 9. 异常处理，打印错误信息。
>
> 该程序可以检查NX部件中未附着到模型的混合和倒角特征，有助于提高模型质量。
>
