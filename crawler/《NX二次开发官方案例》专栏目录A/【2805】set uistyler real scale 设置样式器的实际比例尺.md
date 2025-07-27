### 【2805】set uistyler real scale 设置样式器的实际比例尺

#### 代码

```cpp
    /*HEAD SET_UISTYLER_REAL_SCALE CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #define GTAC_SCALE_REAL_0              ("SCALE_REAL_0")  
    struct limit_s  
    {  
        double  
            minmax[2];  
        char  
            lower[33],  
            upper[33];  
    };  
    typedef struct limit_s limit_t, *limit_p_t;  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：注意，UF_print_syslog 是 V18 中新增的，请只回答翻译，不要添加其他内容。 */  
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
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        UF_STYLER_item_value_type_t  
            set_value;  
        limit_p_t  
            limits = (limit_p_t)client_data;  
        char  
            *labels[2];  
        labels[0] = limits->lower;  
        labels[1] = limits->upper;  
        sprintf(limits->lower, "%.1f", limits->minmax[0]);  
        sprintf(limits->upper, "%.1f", limits->minmax[1]);  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id =  GTAC_SCALE_REAL_0;  
        set_value.value.strings = labels;  
        set_value.count = 2;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id =  GTAC_SCALE_REAL_0;  
        set_value.indicator = UF_STYLER_REAL_PTR_VALUE;  
        set_value.value.reals = limits->minmax;  
        set_value.count = 2;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id =  GTAC_SCALE_REAL_0;  
        set_value.value.real = (limits->minmax[0] + limits->minmax[1]) / 2;  
        set_value.count = 1;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static int GTAC_apply_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        double  
            half;  
        limit_p_t  
            limits = (limit_p_t)client_data;  
        UF_STYLER_item_value_type_t  
            ask_value,  
            set_value;  
        ask_value.item_attr = UF_STYLER_VALUE;  
        ask_value.item_id =  GTAC_SCALE_REAL_0;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &ask_value));  
        half = (limits->minmax[0] + limits->minmax[1]) / 2;  
        limits->minmax[0] = ask_value.value.real - half;  
        limits->minmax[1] = ask_value.value.real + half;  
        sprintf(limits->lower, "%.1f", limits->minmax[0]);  
        sprintf(limits->upper, "%.1f", limits->minmax[1]);  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.item_id =  GTAC_SCALE_REAL_0;  
        set_value.indicator = UF_STYLER_REAL_PTR_VALUE;  
        set_value.value.reals = limits->minmax;  
        set_value.count = 2;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static logical prompt_for_two_numbers(char *prompt, char *item1, char *item2,  
        double *number1, double *number2)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[2][16];  
        double  
            da[2];  
        strcpy(&menu[0][0], item1);  
        strcpy(&menu[1][0], item2);  
        da[0] = *number1;  
        da[1] = *number2;  
        resp = uc1609(prompt, menu, 2, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number1 = da[0];  
            *number2 = da[1];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        limit_t  
            limits = { { 0.0, 10.0 }, "", "" };  
        UF_STYLER_callback_info_t GTAC_cbs[3] = {  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, GTAC_const_cb},  
            {UF_STYLER_DIALOG_INDEX, UF_STYLER_APPLY_CB, 0, GTAC_apply_cb},  
            {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 }};  
        while (prompt_for_two_numbers("Enter scale limits", "Left", "Right",  
            &limits.minmax[0], &limits.minmax[1]) &&  
            !UF_CALL(UF_STYLER_create_dialog("gtac.dlg", GTAC_cbs, 3, &limits,  
            &resp)) && (resp == UF_UI_OK));  
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

> 这段代码是一个NX二次开发示例，主要功能是创建一个对话框，用于设置和获取实数范围。以下是代码的详细介绍：
>
> 1. 头文件和宏定义：代码包含了必要的头文件，并定义了两个宏GTAC_SCALE_REAL_0和UF_CALL，分别用于标识实数滑块控件的ID和记录函数调用结果。
> 2. 数据结构：定义了一个limit_t结构体，用于存储实数范围的最小值、最大值和标签。
> 3. 错误报告函数：定义了report_error函数，用于在函数调用失败时打印错误信息。
> 4. 回调函数：GTAC_const_cb：在对话框创建时调用，用于设置实数滑块的最小值、最大值和默认值。GTAC_apply_cb：在对话框应用时调用，用于获取用户设置的实数滑块值，并更新最小值和最大值。
> 5. GTAC_const_cb：在对话框创建时调用，用于设置实数滑块的最小值、最大值和默认值。
> 6. GTAC_apply_cb：在对话框应用时调用，用于获取用户设置的实数滑块值，并更新最小值和最大值。
> 7. 提示函数：定义了prompt_for_two_numbers函数，用于提示用户输入实数范围的最小值和最大值。
> 8. 主函数：定义了do_it函数，循环提示用户输入范围，并创建对话框。对话框关闭后，继续提示用户重新输入。
> 9. NX启动和关闭函数：ufusr函数用于NX启动时初始化并调用主函数，ufusr_ask_unload函数用于NX关闭时卸载。
> 10. 函数调用封装：使用宏UF_CALL封装函数调用，用于记录错误码并调用错误报告函数。
>
> 综上，这段代码实现了一个简单的对话框，用于设置和获取实数范围，展示了NX二次开发中的回调函数、数据结构、错误处理等基本用法。
>
