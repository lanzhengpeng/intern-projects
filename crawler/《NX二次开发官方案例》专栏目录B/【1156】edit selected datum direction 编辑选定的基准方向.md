### 【1156】edit selected datum direction 编辑选定的基准方向

#### 代码

```cpp
    /*HEAD EDIT_SELECTED_DATUM_DIRECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的功能。 */  
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
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            dirr,  
            feat;  
        double  
            origin[3],  
            direction[3];  
        while (((feat = select_a_feature("Edit datum direction")) != NULL_TAG) &&  
            specify_vector("New direction", direction, origin))  
        {  
            UF_CALL(UF_SO_create_dirr_doubles(feat, UF_SO_dont_update, direction,  
                &dirr));  
            if (!UF_CALL(UF_MODL_edit_datum_direction(feat, dirr)))  
                UF_CALL(UF_MODL_update());  
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

> 根据代码的内容，这是一段用于NX CAD系统的二次开发代码，主要功能是编辑选定特征的基准方向。以下是代码的主要内容和功能：
>
> 1. 头文件：包含了NX二次开发所需的主要头文件，如uf.h、uf_ui.h等。
> 2. 错误处理函数：定义了report_error函数，用于在发生UF函数调用错误时，报告错误信息。
> 3. 选择特征函数：定义了select_a_feature函数，用于提示用户选择一个特征。
> 4. 指定方向函数：定义了specify_vector函数，用于提示用户指定一个方向向量。
> 5. 编辑方向主函数：定义了do_it函数，循环选择特征并指定新方向，然后调用UF_MODL_edit_datum_direction函数来编辑特征的方向。
> 6. ufusr函数：NX二次开发的入口函数，初始化后调用do_it函数，最后终止。
> 7. 卸载函数：定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 综上所述，这段代码实现了交互式选择特征并编辑其基准方向的功能，是NX CAD系统二次开发的一个典型示例。
>
