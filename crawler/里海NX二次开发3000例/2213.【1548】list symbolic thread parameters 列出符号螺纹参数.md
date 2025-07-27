### 【1548】list symbolic thread parameters 列出符号螺纹参数

#### 代码

```cpp
    /*HEAD LIST_SYMBOLIC_THREAD_PARAMETERS CCC UFUN */  
    #include <stdlib.h>  
    #include <string.h>  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_defs.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_cfi.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能，只回答翻译，不要添加任何其他内容。 */  
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
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat));  
        return feat;  
    }  
    static void do_it(void)  
    {  
        char  
            msg[133],  
            *feat_type;  
        tag_t  
            feat_tag = NULL_TAG,  
            part_tag =  UF_PART_ask_display_part();  
        UF_MODL_symb_thread_data_t  
            symb_param;  
        double  
            feat_loc[3];  
        while ((feat_tag = ask_next_feature(part_tag, feat_tag)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat_tag, &feat_type));  
            if(!strcmp(feat_type , "SYMBOLIC_THREAD"))  
            {  
                sprintf(msg, "\tFeature tag : %d\n", feat_tag);  
                if (!UF_UI_open_listing_window())  
                    UF_UI_write_listing_window(msg);  
                UF_CALL(UF_MODL_ask_symb_thread_parms(feat_tag, &symb_param));  
                sprintf(msg, "callout is: _%s_\n", symb_param.callout);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "length is: _%s_\n", symb_param.length);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "form is: _%s_\n", symb_param.form);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "method is: _%s_\n", symb_param.method);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "major_dia is: _%s_\n", symb_param.major_dia);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "minor_dia is: _%s_\n", symb_param.minor_dia);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "pitch is: _%s_\n", symb_param.pitch);  
                UF_UI_write_listing_window(msg);  
                sprintf(msg, "angle is: _%s_\n", symb_param.angle);  
                UF_UI_write_listing_window(msg);  
                UF_MODL_ask_feat_location(feat_tag, feat_loc);  
                sprintf(msg, "thread feature located at: %f, %f, %f",  
                    feat_loc[0], feat_loc[1], feat_loc[2] );  
                UF_UI_write_listing_window(msg);  
            }  
            UF_free(feat_type);  
        }  
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
            sprintf(msg, "    %s - %s\n", status->file_names[ii], msg);  
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

> 这段代码是NX的二次开发代码，主要功能是遍历当前显示的零件中的所有特征，并筛选出符号螺纹特征，然后获取并输出这些符号螺纹特征的相关参数信息。
>
> 代码的主要逻辑包括：
>
> 1. 定义了一个错误报告函数report_error，用于输出错误信息。
> 2. 定义了一个函数ask_next_feature，用于遍历零件中的下一个特征。
> 3. 定义了一个函数do_it，用于遍历所有特征，筛选出符号螺纹特征，并获取其参数信息，包括调用、长度、形式、方法、大径、小径、螺距和角度等，并输出这些信息。
> 4. 定义了主函数ufusr，用于初始化NX，调用do_it函数，然后终止NX。
> 5. 定义了主函数main，用于处理命令行参数，打开指定路径的零件，调用do_it函数，最后输出处理过的零件数量。
> 6. 定义了一个检查加载状态的函数check_load_status，用于检查并输出零件的加载状态信息。
>
> 总的来说，这段代码实现了遍历零件特征，筛选并获取符号螺纹特征参数信息的功能，并提供了命令行运行和UG内部运行两种方式。
>
