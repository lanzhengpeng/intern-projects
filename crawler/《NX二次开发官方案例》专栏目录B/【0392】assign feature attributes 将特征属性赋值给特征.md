### 【0392】assign feature attributes 将特征属性赋值给特征

#### 代码

```cpp
    /*HEAD ASSIGN_FEATURE_ATTRIBUTES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_attr.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:翻译：
注意：UF_print_syslog是V18版本新增的，请只提供翻译，不要添加其他内容。 */  
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
    static void report_attributes(tag_t object)  
    {  
        int  
            indx = 0,  
            n;  
        char  
            dstr[21],  
            mstr[UF_UI_MAX_STRING_LEN+1],  
            title[UF_ATTR_MAX_TITLE_LEN+1],  
            tstr[21];  
        UF_ATTR_value_t  
            att;  
        UF_CALL(UF_UI_open_listing_window());  
        UF_CALL(UF_ATTR_count_attributes(object, UF_ATTR_any, &n));  
        sprintf(mstr, "tag %d has %d attributes\n", object, n);  
        UF_CALL(UF_UI_write_listing_window(mstr));  
        while (!UF_CALL(UF_ATTR_cycle(object, &indx, UF_ATTR_any, title, &att))  
            && indx)  
        {  
            switch ( att.type )  
            {  
                case UF_ATTR_integer:  
                    sprintf(mstr, "%s = %d (INTEGER)\n", title, att.value.integer);  
                break;  
                case UF_ATTR_real:  
                    sprintf(mstr, "%s = %f (FLOATING POINT)\n", title,  
                        att.value.real);  
                break;  
                case UF_ATTR_time:  
                    UF_CALL(uc4582(att.value.time, 2, dstr, tstr));  
                    sprintf(mstr, "%s = %s %s (DATE & TIME)\n", title, dstr, tstr);  
                break;  
                case UF_ATTR_null:  
                    sprintf(mstr, "%s = (NULL)\n", title);  
                break;  
                case UF_ATTR_string:  
                    sprintf(mstr, "%s = %s (STRING)\n", title, att.value.string);  
                    UF_free(att.value.string);  
                break;  
                case UF_ATTR_reference:  
                    sprintf(mstr, "%s = %s (REFERENCE)\n", title,  
                        att.value.reference);  
                    UF_free(att.value.reference);  
                break;  
                default:  
                break;  
            }  
            UF_CALL(UF_UI_write_listing_window(mstr));  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        UF_ATTR_value_t  
            value;  
        value.type = UF_ATTR_string;  
        value.value.string = "The value of the test attribute";  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
        {  
            UF_CALL(UF_ATTR_assign(feat, "Test", value));  
            report_attributes(feat);  
        }  
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

```

#### 代码解析

> 这段NX二次开发代码的主要功能是给NX部件特征赋值属性，并报告这些属性。
>
> 代码主要包括以下部分：
>
> 1. 头文件包含：包含必要的NX API头文件，如uf.h、uf_ui.h等。
> 2. 错误报告函数：用于在API调用出错时，记录错误信息。
> 3. 选择特征函数：通过UF_UI_select_feature选择一个特征，返回特征标签。
> 4. 报告属性函数：通过UF_ATTR_count_attributes和UF_ATTR_cycle遍历特征的所有属性，并使用UF_UI_write_listing_window输出属性信息。
> 5. 赋值属性函数：通过UF_ATTR_assign给特征赋值一个字符串类型的属性。
> 6. 主函数：循环调用选择特征函数，获取特征标签，然后调用赋值属性函数，并报告属性。
> 7. UFusr函数：NX二次开发程序的入口函数，用于初始化和终止NX API，并调用主函数。
> 8. 卸载函数：用于在二次开发程序卸载时调用。
>
> 总的来说，这段代码通过NX API实现了一个简单的二次开发程序，能够给NX特征赋值属性，并输出这些属性信息。
>
