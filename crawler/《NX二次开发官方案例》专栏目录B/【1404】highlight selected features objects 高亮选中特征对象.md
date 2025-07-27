### 【1404】highlight selected features objects 高亮选中特征对象

#### 代码

```cpp
    /*HEAD HIGHLIGHT_SELECTED_FEATURES_OBJECTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog在V18版本中是新增的功能。 */  
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
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            feat,  
            *kids;  
        char  
            msg[133];  
        while ((feat = select_a_feature("Select a Feature")) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_feat_object(feat, &n, &kids));  
            set_highlight_object_array(n, kids, TRUE);  
            sprintf(msg, "feature has %d object(s)", n);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n, kids, FALSE);  
            if (n > 0) UF_free(kids);  
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

> 这段NX二次开发代码主要包括以下功能：
>
> 1. 错误报告函数：report_error函数用于输出错误信息，包括错误代码、调用函数名称和错误消息，同时支持在日志窗口和syslog中输出。
> 2. 选择特征函数：select_a_feature函数用于提示用户选择一个特征，并返回第一个选择的特征标签。
> 3. 高亮显示对象数组函数：set_highlight_object_array函数用于设置对象数组的高亮显示，其中flag参数决定是开启还是关闭高亮。
> 4. 执行函数：do_it函数用于循环选择特征，获取特征对应的对象数组，并设置高亮显示，然后提示对象数量，最后关闭高亮显示。
> 5. ufusr主函数：用于初始化和执行do_it函数，然后终止NX。
> 6. 卸载询问函数：ufusr_ask_unload函数用于立即卸载NX二次开发应用程序。
>
> 总体来说，这段代码实现了选择特征，获取特征对象，并高亮显示特征对象的功能，通过交互方式让用户选择特征，并在界面上高亮显示所选特征的对象，以实现交互式特征查看的功能。
>
