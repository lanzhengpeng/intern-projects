### 【2327】report selected enlarge feature parameters 报告选定的放大特征参数

#### 代码

```cpp
    /*HEAD REPORT_SELECTED_ENLARGE_FEATURE_PARAMETERS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解UF_print_syslog是在V18版本中新增的一个功能。根据要求，只提供翻译内容，不添加任何废话。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    #define WRITEnS(n,X) (write_string_array_to_listing_window(#X, X, n))  
    void write_string_array_to_listing_window(char *title, char **strings,  
        int n_strings)  
    {  
        int  
            ii;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_strings; ii++)  
        {  
            sprintf(msg, "%s[%d] = %s\n", title, ii, strings[ii]);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            type;  
        tag_t  
            face,  
            feature;  
        double  
            tolerance;  
        char  
            *percent_size[4];  
        while ((feature = select_a_feature("Report Enlarge")) != NULL_TAG)  
        {  
            if (UF_CALL(UF_MODL_ask_enlarge(feature, &face, &type, percent_size,  
                &tolerance))) continue;  
            WRITE_D(face);  
            WRITE_D(type);  
            WRITEnS(4, percent_size);  
            WRITE_F(tolerance);  
            for (ii = 0; ii < 4; ii++) UF_free(percent_size[ii]);  
        }  
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

> 这是段用于报告NX特征放大参数的二次开发代码，其主要功能包括：
>
> 1. 报告错误：定义了report_error函数，用于报告NX API调用错误，并输出错误信息。
> 2. 写整数/浮点数/字符串数组到列表窗口：定义了write_integer_to_listing_window、write_double_to_listing_window和write_string_array_to_listing_window函数，用于将整数、浮点数和字符串数组写入NX的列表窗口。
> 3. 选择特征：定义了select_a_feature函数，用于提示用户选择特征。
> 4. 报告特征放大参数：在do_it函数中，循环调用select_a_feature选择特征，然后调用UF_MODL_ask_enlarge函数获取特征的放大参数，并输出到列表窗口。
> 5. UFusr入口函数：定义了ufusr函数，作为NX的UFusr入口函数，在NX启动时调用，完成初始化后调用do_it函数，然后终止NX。
> 6. 卸载函数：定义了ufusr_ask_unload函数，用于立即卸载该二次开发代码。
>
> 总的来说，这段代码通过二次开发获取并报告了NX特征放大参数，实现了自定义的参数输出功能。
>
