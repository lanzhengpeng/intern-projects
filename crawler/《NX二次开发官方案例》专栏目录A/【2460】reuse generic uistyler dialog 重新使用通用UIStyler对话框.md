### 【2460】reuse generic uistyler dialog 重新使用通用UIStyler对话框

#### 代码

```cpp
    /*HEAD REUSE_GENERIC_UISTYLER_DIALOG CCC UFUN */  
    /*  To create your own dialog for use with this program:  
        Application->UIStyler  
        change CHANGE to MY  
        set Button Style Options to Ok, Back and Cancel  
        set OK, Back and Cancel Initially Sensitive  
        enter ok callback:  MY_ok_cb  
        enter constructor callback:  MY_const_cb  
        add a label object  
        add an integer object  
        add a real object  
        add a real object  
        File->Save As->my_generic  
    */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据上下文，UF_print_syslog在V18版本中是新增的，请直接翻译为：

UF_print_syslog在V18版本中是新增的。 */  
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
    struct MY_dialog_data_s  
    {  
        int  
            integer;  
        char  
            *cue,  
            *integer_label,  
            *main_label,  
            *real_label[2],  
            *title;  
        double  
            real[2];  
    };  
    typedef struct MY_dialog_data_s MY_dialog_data_t, *MY_dialog_data_p_t;  
    #define MY_CB_COUNT ( 3 + 1 ) /* Add 1 for the terminator 里海译:在终结符后加1 */  
    #define MY_LABEL_0                     ("LABEL_0")  
    #define MY_INT_1                       ("INT_1")  
    #define MY_REAL_2                      ("REAL_2")  
    #define MY_REAL_3                      ("REAL_3")  
    /* qq3123197280 */  
    static int MY_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        MY_dialog_data_p_t  
            the_data = (MY_dialog_data_p_t) client_data;  
        UF_STYLER_item_value_type_t  
            set_value;  
    /*  Set the title on the dialog 里海译:将标题设置在对话框上 */  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = UF_STYLER_DIALOG_INDEX;  
        set_value.value.string = the_data->title;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    /*  Set the cue for the dialog 里海译:Set the cue for the dialog的翻译为“为对话设定提示”。 */  
        UF_CALL(UF_UI_set_prompt(the_data->cue));  
    /*  Set the main label 里海译:设置主标签 */  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = MY_LABEL_0;  
        set_value.value.string = the_data->main_label;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    /*  Set the label and value on the integer 里海译:设置整数上的标签和值 */  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = MY_INT_1;  
        set_value.value.string = the_data->integer_label;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.value.integer = the_data->integer;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
    /*  Set the labels and values on the reals 里海译:在实数上设置标签和值。 */  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = MY_REAL_2;  
        set_value.value.string = the_data->real_label[0];  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.value.real = the_data->real[0];  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_LABEL;  
        set_value.item_id = MY_REAL_3;  
        set_value.value.string = the_data->real_label[1];  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        set_value.item_attr = UF_STYLER_VALUE;  
        set_value.value.real = the_data->real[1];  
        UF_CALL(UF_STYLER_set_value(dialog_id, &set_value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    static void store_my_data(int dialog_id, MY_dialog_data_p_t the_data)  
    {  
        int  
            n;  
        UF_STYLER_item_value_type_t  
            ask_value[3];  
        ask_value[0].item_attr = UF_STYLER_VALUE;  
        ask_value[0].item_id = MY_INT_1;  
        ask_value[1].item_attr = UF_STYLER_VALUE;  
        ask_value[1].item_id = MY_REAL_2;  
        ask_value[2].item_attr = UF_STYLER_VALUE;  
        ask_value[2].item_id = MY_REAL_3;  
        UF_CALL(UF_STYLER_ask_values(dialog_id, 3, ask_value, &n));  
        the_data->integer = ask_value[0].value.integer;  
        the_data->real[0] = ask_value[1].value.real;  
        the_data->real[1] = ask_value[2].value.real;  
    }  
    /* qq3123197280 */  
    static int MY_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        store_my_data(dialog_id, (MY_dialog_data_p_t) client_data);  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void do_it(void)  
    {  
        int  
            resp;  
        MY_dialog_data_t  
            one_data,  
            two_data;  
        UF_STYLER_callback_info_t  
            MY_cbs[MY_CB_COUNT] = {  
                {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB, 0, MY_const_cb},  
               {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB         , 0, MY_ok_cb},  
                {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
        one_data.title = "First question";  
        one_data.cue = "Enter primary data";  
        one_data.main_label = "Feature data";  
        one_data.integer_label = "Number";  
        one_data.real_label[0] = "Width";  
        one_data.real_label[1] = "Height";  
        one_data.integer = 1;  
        one_data.real[0] = 2.0;  
        one_data.real[1] = 3.0;  
        two_data.title = "Second question";  
        two_data.cue = "Enter additional data";  
        two_data.main_label = "Project duration";  
        two_data.integer_label = "Days";  
        two_data.real_label[0] = "Hours";  
        two_data.real_label[1] = "Minutes";  
        two_data.integer = 10;  
        two_data.real[0] = 20.0;  
        two_data.real[1] = 30.0;  
        while (  
            !UF_CALL(UF_STYLER_create_dialog("my_generic.dlg", MY_cbs,  
                MY_CB_COUNT, (void *)&one_data, &resp))  
            && (resp == UF_UI_OK) &&  
            !UF_CALL(UF_STYLER_create_dialog("my_generic.dlg", MY_cbs,  
                MY_CB_COUNT, (void *)&two_data, &resp))  
            && (resp == UF_UI_BACK)  
            ) ;  
        if (resp != UF_UI_OK) return;  
        WRITE_D(one_data.integer);  
        WRITE_F(one_data.real[0]);  
        WRITE_F(one_data.real[1]);  
        WRITE_D(two_data.integer);  
        WRITE_F(two_data.real[0]);  
        WRITE_F(two_data.real[1]);  
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

> 这段代码是基于NX的二次开发代码，实现了以下功能：
>
> 1. 定义了一个自定义对话框my_generic，其中包含标签、整数输入、实数输入等控件。
> 2. 创建了两个对话框实例one_data和two_data，分别用于输入不同的数据。
> 3. 实现了对话框的构造回调函数MY_const_cb，用于初始化对话框的标题、提示信息和控件标签。
> 4. 实现了OK按钮的回调函数MY_ok_cb，用于在用户点击OK时保存输入的数据。
> 5. 主函数do_it中循环创建并显示两个对话框，直到用户点击OK按钮。
> 6. 用户点击OK后，将输入的数据输出到列表窗口。
> 7. 定义了ufusr函数作为程序的入口点，用于初始化和调用主函数。
> 8. 定义了ufusr_ask_unload函数，用于立即卸载用户自定义程序。
> 9. 使用了错误报告函数report_error，用于输出函数调用失败的错误信息。
> 10. 整体流程清晰，实现了自定义对话框的创建、显示、输入数据收集和输出。
>
