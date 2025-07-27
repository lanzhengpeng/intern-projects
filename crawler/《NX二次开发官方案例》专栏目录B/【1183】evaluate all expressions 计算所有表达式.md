### 【1183】evaluate all expressions 计算所有表达式

#### 代码

```cpp
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /*  There is no API method equvalent to these interactive options:  
            Tools-> Update-> Update for External Change  
            Tools-> Expression-> Refresh values from external spreadsheet  
            Tools-> Spreadsheet-> Edit-> Update NX part  
        This program demonstrates a method which worked to update the model  
        in a test case after changing values in an external spreadsheet.  
    */  
    static logical evaluate_all_expressions(tag_t part, logical verbose)  
    {  
        logical  
            something_changed = FALSE;  
        int  
            ii,  
            n_exp;  
        tag_t  
            *exps;  
        double  
            new_value,  
            old_value;  
        char  
            *full_string;  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exp, &exps));  
        if (n_exp > 0)  
        {  
            for (ii = 0; ii < n_exp; ii++)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &full_string));  
                if (verbose) WRITE(full_string);  
                if (verbose) WRITE("\n");  
                UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &old_value));  
                if (verbose) WRITE_F(old_value);  
                UF_CALL(UF_MODL_edit_exp(full_string));  
                UF_free(full_string);  
                UF_CALL(UF_MODL_ask_exp_tag_value(exps[ii], &new_value));  
                if (verbose) WRITE_F(new_value);  
                if (new_value != old_value) something_changed = TRUE;  
            }  
            if (verbose) WRITE_L(something_changed);  
            UF_free(exps);  
        }  
        return something_changed;  
    }

```

#### 代码解析

> 这段代码是NX Open API的二次开发代码，用于评估和更新NX部件中的表达式。具体来说：
>
> 1. 定义了三个宏，用于在列表窗口中写入不同类型的信息，分别是字符串、浮点数和逻辑值。
> 2. 定义了一个函数evaluate_all_expressions，用于评估部件中所有表达式的值，并判断是否有变化。
> 3. 该函数首先获取部件中的所有表达式，然后逐个评估表达式的值。评估前会输出表达式的字符串和当前值，然后重新评估表达式，再次输出新的值。
> 4. 如果新的值与旧值不同，表示表达式值有变化，函数返回true。如果所有表达式值都没有变化，则返回false。
> 5. 该函数可用于检测外部参数变化后，NX部件中的表达式是否需要更新。如果有变化，可以进一步调用NX API更新相关参数，从而实现外部参数与NX模型的联动。
> 6. 虽然代码中注释提到了NX工具栏中没有直接对应API的方法，但这段代码实现了类似的功能，即评估表达式并判断是否需要更新。
> 7. 这段代码是NX二次开发中的一个典型示例，展示了如何利用NX Open API实现参数化设计，以及如何检测和响应外部参数的变化。
>
