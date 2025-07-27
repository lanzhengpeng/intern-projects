### 【1894】report all c sunk hole parameters 报告所有沉头孔参数

#### 代码

```cpp
    /*HEAD REPORT_ALL_C_SUNK_HOLE_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本新增的函数，用于打印系统日志。文档要求只提供翻译，不添加任何额外内容。 */  
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
    static tag_t ask_next_c_sunk_hole(tag_t part, tag_t feat)  
    {  
        char  
            *type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
                if (!strcmp(type, "CSUNK_HOLE"))  
                {  
                    UF_free(type);  
                    return feat;  
                }  
                UF_free(type);  
            }  
        }  
        return NULL_TAG;  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %s\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_c_sunk_hole_parms(tag_t c_sunk)  
    {  
        int  
            ii,  
            n_exps,  
            thru;  
        char  
            *csink_angle,  
            *depth1,  
            **descs,  
            *diameter1,  
            *diameter2,  
            *string,  
            *tip_angle;  
        tag_t  
            *exps;  
        WRITE_D(c_sunk);  
        WRITE("\nUsing UF_MODL_ask_c_sunk_hole_parms:\n");  
        UF_CALL(UF_MODL_ask_c_sunk_hole_parms(c_sunk, 0, &diameter1, &diameter2,  
            &depth1, &csink_angle, &tip_angle, &thru));  
        WRITE_S(diameter1);  
        WRITE_S(diameter2);  
        WRITE_S(depth1);  
        WRITE_S(csink_angle);  
        WRITE_S(tip_angle);  
        WRITE_L(thru);  
        UF_free(diameter1);  
        UF_free(diameter2);  
        UF_free(depth1);  
        UF_free(csink_angle);  
        UF_free(tip_angle);  
        WRITE("\nUsing UF_MODL_ask_exp_desc_of_feat:\n");  
        UF_CALL(UF_MODL_ask_exp_desc_of_feat(c_sunk, &n_exps, &descs, &exps));  
        WRITE_D(n_exps);  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(exps[ii]);  
            WRITE_S(descs[ii]);  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
            WRITE_S(string);  
            UF_free(string);  
        }  
        UF_free(exps);  
        UF_free_string_array(n_exps, descs);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            hole = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        while ((hole = ask_next_c_sunk_hole(part, hole)) != NULL_TAG)  
            report_c_sunk_hole_parms(hole);  
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

```

#### 代码解析

> 这段NX二次开发代码的主要功能是遍历当前活动部件中的所有沉孔特征，并报告每个沉孔的特征参数。具体步骤如下：
>
> 1. 定义了一个报告错误函数report_error，用于在发生错误时输出错误信息。
> 2. 定义了ask_next_c_sunk_hole函数，用于遍历部件中的下一个沉孔特征。
> 3. 定义了几个辅助函数，用于向列表窗口输出不同类型的数据。
> 4. 定义了report_c_sunk_hole_parms函数，用于报告一个沉孔的特征参数，包括直径、深度、角度等。
> 5. 定义了do_it函数，用于遍历当前活动部件中的所有沉孔特征，并调用report_c_sunk_hole_parms函数报告每个沉孔的特征参数。
> 6. 定义了ufusr函数，这是NX二次开发的入口函数。它初始化NX环境，调用do_it函数完成主要功能，然后终止NX环境。
> 7. 定义了ufusr_ask_unload函数，用于卸载用户自定义函数。
>
> 总的来说，这段代码通过遍历并报告每个沉孔的特征参数，实现了对部件中沉孔特征的查询。
>
