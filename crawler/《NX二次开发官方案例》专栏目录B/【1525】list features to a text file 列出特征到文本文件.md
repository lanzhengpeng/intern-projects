### 【1525】list features to a text file 列出特征到文本文件

#### 代码

```cpp
    /*HEAD LIST_FEATURES_TO_A_TEXT_FILE CCC UFUN */  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文中的信息，UF_print_syslog 是 V18 版本新增的函数。

因此，翻译为：

UF_print_syslog 是 V18 版本新增的函数。 */  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE) return feat;  
        return NULL_TAG;  
    }  
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0,  
            resp;  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        char  
            fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            msg[MAX_LINE_SIZE+1],  
            *name,  
            *type;  
        build_similar_filespec(fspec, 0);  
        strcat(fspec, "_features.txt");  
        uc4400(4, fspec, 0);  
        FTN(uf4401)(&resp);  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &type ));  
            UF_CALL(UF_MODL_ask_feat_name(feat, &name ));  
            sprintf(msg, "%d.  %s (%s)", ++ii, name, type);  
            UF_CALL(uc4403(msg));  
            UF_free(type);  
            UF_free(name);  
        }  
        uc4400(0, "", 0);  
    }  
    /* qq3123197280 */  
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
            part_name[UF_CFI_MAX_PATH_NAME_SIZE];  
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

> 这段代码是一个NX Open C++应用程序，主要功能是将当前活动零件的所有特征名称和类型输出到一个文本文件中。
>
> 代码的主要流程如下：
>
> 1. 定义了report_error函数，用于报告UF函数调用失败时的错误信息。
> 2. 定义了ask_next_feature函数，用于遍历零件中的所有特征，并返回下一个有效的特征对象。
> 3. 定义了build_similar_filespec函数，用于根据当前零件名构建输出文本文件的路径。
> 4. 定义了do_it函数，用于遍历零件中的所有特征，获取每个特征的名称和类型，并输出到文本文件中。
> 5. 定义了ufusr函数，是NX Open C++应用程序的入口函数。在ufusr中，初始化UF模块，调用do_it函数，然后终止UF模块。
> 6. 定义了ufusr_ask_unload函数，用于设置应用程序卸载模式。
> 7. 定义了check_load_status函数，用于检查零件加载状态。
> 8. 在main函数中，处理命令行参数，打开每个零件，调用do_it函数，最后关闭所有零件。
>
> 这个程序的主要功能是遍历零件中的所有特征，将每个特征的名称和类型输出到一个与零件同名的文本文件中。通过命令行参数可以打开多个零件文件，并对每个零件执行同样的操作。
>
