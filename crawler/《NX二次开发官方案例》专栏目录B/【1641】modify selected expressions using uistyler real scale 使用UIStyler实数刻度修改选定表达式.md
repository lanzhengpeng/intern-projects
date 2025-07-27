### 【1641】modify selected expressions using uistyler real scale 使用UIStyler实数刻度修改选定表达式

#### 代码

```cpp
    /*  This program demonstrates modifying selected expressions based on the  
        value specified using a real scale UIStyler object.  
        To create a dialog to use with this program:  
        - Select Application->User Interface Styler  
        - Set the Dialog Title to "Modify expressions"  
        - Set the Cue to "select expression or change value"  
        - Change the dialog's Prefix Name to "GTAC"  
        - Set the Navigation buttons to Back with Back initially sensitive  
        - Specify the Constructor Callback as GTAC_"const_cb"  
        - Specify the Ok Callback as GTAC_"ok_cb"  
        - Specify the Back Callback as GTAC_"back_cb"  
        - Specify the Cancel Callback as GTAC_"cancel_cb"  
        - Add a Single Select List  
        - Set its Activate callback to "new_exp_cb"  
        - Add a Label  
        - Add a Real Scale  
        - Specify its Drag callback as "new_value_cb"  
        - Specify its Value Changed callback as "new_value_cb"  
        - Add a Label "Scale increment - power of 10"  
        - Add an Integer Scale  
        - Set the intial value -1  
        - Set the limits -4 to 4  
        - Specify its Drag callback as "new_precision_cb"  
        - Specify its Value Changed callback as "new_precision_cb"  
        - Save Dialog As "slide_expression"  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <math.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_styler.h>  
    #include <uf_modl.h>  
    #include <uf_undo.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #define GTAC_CB_COUNT 10  
    #define GTAC_LIST_0                    ("LIST_0")  
    #define GTAC_LABEL_1                   ("LABEL_1")  
    #define GTAC_SCALE_REAL_2              ("SCALE_REAL_2")  
    #define GTAC_LABEL_3                   ("LABEL_3")  
    #define GTAC_SCALE_INT_4               ("SCALE_INT_4")  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数。 */  
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
    struct EXP_data_s  
    {  
        int  
            dialog_id,  
            precision,  
            save_mark,  
            temp_mark;  
        tag_t  
            feat;  
        char  
            exp[UF_MAX_EXP_LENGTH];  
        double  
            value;  
    };  
    typedef struct EXP_data_s EXP_data_t, *EXP_data_p_t;  
    static tag_t ask_tag_of_expression_name(char *exp_name)  
    {  
        tag_t  
            exp_tag;  
        char  
            exp_str[UF_MAX_EXP_LENGTH+1] = { "" },  
            *lhs,  
            *rhs;  
        UF_CALL(UF_MODL_ask_exp(exp_name, exp_str));  
        if (!UF_CALL(UF_MODL_dissect_exp_string(exp_str, &lhs, &rhs, &exp_tag)))  
        {  
            UF_free(lhs);  
            UF_free(rhs);  
            return (exp_tag);  
        }  
        else  
            return NULL_TAG;  
    }  
    static void build_expression_description(EXP_data_p_t data, char *description)  
    {  
        int  
            n_feats;  
        tag_t  
            exp_tag,  
            *feats;  
        char  
            *desc,  
            *feat_name;  
        exp_tag = ask_tag_of_expression_name(data->exp);  
        UF_CALL(UF_MODL_ask_descriptor_of_exp(exp_tag, &desc));  
        if (desc != NULL)  
        {  
            sprintf(description, "%s - %s", data->exp, desc);  
            UF_free(desc);  
        }  
        else  
            sprintf(description, "%s - ?", data->exp);  
        UF_CALL(UF_MODL_ask_features_of_exp(exp_tag, &n_feats, &feats));  
        if (n_feats > 0)  
        {  
            data->feat = feats[0];  
            UF_CALL(UF_MODL_ask_feat_name(feats[0], &feat_name));  
            strcat(description, " for ");  
            strcat(description, feat_name);  
            UF_free(feats);  
            UF_free(feat_name);  
        }  
        else  
            data->feat = NULL_TAG;  
    }  
    void set_scale_by_exp(EXP_data_p_t data)  
    {  
        double  
            limits[2];  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_STYLER_item_value_type_t  
            value;  
        UF_CALL(UF_MODL_eval_exp(data->exp, &data->value));  
        limits[0] = data->value - pow(10, data->precision + 1);  
        limits[1] = data->value + pow(10, data->precision + 1);  
        value.item_id = GTAC_SCALE_REAL_2;  
        value.item_attr  = UF_STYLER_VALUE;  
        value.indicator = UF_STYLER_REAL_PTR_VALUE;  
        value.count = 2;  
        value.value.reals = limits;  
        UF_CALL(UF_STYLER_set_value(data->dialog_id, &value));  
        value.count = 1;  
        value.value.real = data->value;  
        UF_CALL(UF_STYLER_set_value(data->dialog_id, &value));  
        value.item_attr  = UF_STYLER_SCALE_PRECISION;  
        if (data->precision > -1)  
            value.value.integer = 0;  
        else  
            value.value.integer = abs(data->precision);  
        UF_CALL(UF_STYLER_set_value(data->dialog_id, &value));  
        build_expression_description(data, msg);  
        value.item_id = GTAC_LABEL_1;  
        value.item_attr  = UF_STYLER_LABEL;  
        value.value.string = msg;  
        UF_CALL(UF_STYLER_set_value(data->dialog_id, &value));  
    }  
    /* qq3123197280 */  
    static int GTAC_const_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            ii,  
            n_exps;  
        tag_t  
            exp_tag,  
            *exps,  
            part = UF_PART_ask_display_part();  
        char  
            *lhs,  
            msg[MAX_LINE_SIZE+1],  
            *rhs,  
            *string;  
        UF_STYLER_item_value_type_t  
            value;  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &data->save_mark));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &data->temp_mark));  
        data->dialog_id = dialog_id;  
        value.item_attr  = UF_STYLER_LIST_INSERT;  
        value.item_id = GTAC_LIST_0;  
        value.subitem_index = UF_STYLER_NO_SUB_INDEX;  
        value.indicator = UF_STYLER_STRING_VALUE;  
        value.count = 1;  
        value.value.string = msg;  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        for (ii = 0; ii < n_exps; ii++)  
        {  
            UF_CALL(UF_MODL_ask_exp_tag_string(exps[ii], &string));  
            UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp_tag));  
            strcpy(data->exp, lhs);  
            build_expression_description(data, msg);  
            UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
            UF_free(string);  
            UF_free(lhs);  
            UF_free(rhs);  
        }  
        UF_CALL(UF_MODL_ask_exp_tag_string(exps[0], &string));  
        UF_CALL(UF_MODL_dissect_exp_string(string, &lhs, &rhs, &exp_tag));  
        strcpy(data->exp, lhs);  
        build_expression_description(data, msg);  
        UF_free(string);  
        UF_free(lhs);  
        UF_free(rhs);  
        UF_free(exps);  
        set_scale_by_exp(data);  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, TRUE));  
        value.item_id = GTAC_LIST_0;  
        value.item_attr  = UF_STYLER_VALUE;  
        value.subitem_index = 0;  
        UF_CALL(UF_STYLER_set_value(dialog_id, &value));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_new_exp_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        UF_STYLER_item_value_type_t  
            value;  
        value.item_id = GTAC_LIST_0;  
        value.item_attr  = UF_STYLER_VALUE;  
        value.indicator = UF_STYLER_STRING_VALUE;  
        UF_CALL(UF_STYLER_ask_value(dialog_id, &value));  
        strcpy(data->exp, strtok(value.value.string, " "));  
        UF_CALL(UF_STYLER_free_value(&value));  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, FALSE));  
        set_scale_by_exp(data);  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, TRUE));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_new_value_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            exists;  
        char  
            exp_str[UF_MAX_EXP_LENGTH];  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        sprintf(exp_str, "%s=%f", data->exp, callback_data->value.real);  
        UF_CALL(UF_MODL_edit_exp(exp_str));  
    /*  value may be invalid - if so set slider back 里海译:翻译如下：
值可能无效 - 如果是，请将滑块设置回。 */  
        if (UF_MODL_update())  
        {  
            sprintf(exp_str, "%s=%f", data->exp, data->value);  
            UF_CALL(UF_MODL_edit_exp(exp_str));  
            callback_data->value.real = data->value;  
            UF_CALL(UF_STYLER_set_value(dialog_id, callback_data));  
        }  
        else  
            data->value = callback_data->value.real;  
        UF_CALL(UF_UNDO_ask_mark_exist(data->temp_mark, NULL, &exists));  
        if (exists)  
        {  
            UF_CALL(UF_UNDO_delete_to_mark(data->temp_mark, NULL));  
            UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &data->temp_mark));  
        }  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, TRUE));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_new_precision_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        data->precision = callback_data->value.integer;  
        set_scale_by_exp(data);  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_ok_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, FALSE));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_back_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            exists;  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        UF_CALL(UF_UNDO_ask_mark_exist(data->save_mark, NULL, &exists));  
        if (exists) UF_CALL(UF_UNDO_undo_to_mark(data->save_mark, NULL));  
        set_scale_by_exp(data);  
        if (data->feat != NULL_TAG)  
            UF_CALL(UF_DISP_set_highlight(data->feat, TRUE));  
        return (UF_UI_CB_CONTINUE_DIALOG);  
    }  
    /* qq3123197280 */  
    static int GTAC_cancel_cb(int dialog_id, void *client_data,  
        UF_STYLER_item_value_type_p_t callback_data)  
    {  
        int  
            exists;  
        EXP_data_p_t  
            data = (EXP_data_p_t)client_data;  
        UF_CALL(UF_UNDO_ask_mark_exist(data->save_mark, NULL, &exists));  
        if (exists) UF_CALL(UF_UNDO_undo_to_mark(data->save_mark, NULL));  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_invisible, NULL, &data->temp_mark));  
        return (UF_UI_CB_EXIT_DIALOG);  
    }  
    static void do_it(void)  
    {  
        int  
            n_exps,  
            resp;  
        tag_t  
            *exps,  
            part = UF_PART_ask_display_part();  
        EXP_data_t  
            data = { 0, -1, 0, 0, NULL_TAG, { "" }, 1.0 };  
        UF_STYLER_callback_info_t  
            GTAC_cbs[GTAC_CB_COUNT] = {  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CONSTRUCTOR_CB  , 0, GTAC_const_cb},  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_OK_CB           , 0, GTAC_ok_cb},  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_BACK_CB         , 0, GTAC_back_cb},  
     {UF_STYLER_DIALOG_INDEX, UF_STYLER_CANCEL_CB       , 0, GTAC_cancel_cb},  
     {GTAC_LIST_0           , UF_STYLER_ACTIVATE_CB     , 0, GTAC_new_exp_cb},  
     {GTAC_SCALE_REAL_2     , UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_new_value_cb},  
     {GTAC_SCALE_REAL_2     , UF_STYLER_DRAG_CB         , 0, GTAC_new_value_cb},  
     {GTAC_SCALE_INT_4      , UF_STYLER_VALUE_CHANGED_CB, 0, GTAC_new_precision_cb},  
     {GTAC_SCALE_INT_4      , UF_STYLER_DRAG_CB         , 0, GTAC_new_precision_cb},  
     {UF_STYLER_NULL_OBJECT, UF_STYLER_NO_CB, 0, 0 } };  
    /*  dialog cannot be aborted from the constructor callback so check here 里海译:翻译：从构造函数回调中无法终止翻译对话框，因此请在此进行检查。 */  
        UF_CALL(UF_MODL_ask_exps_of_part(part, &n_exps, &exps));  
        if (n_exps == 0)  
        {  
            uc1601("Part has no expressions!", TRUE);  
            return;  
        }  
        UF_free(exps);  
        UF_CALL(UF_STYLER_create_dialog("slide_expression", GTAC_cbs,  
            GTAC_CB_COUNT, &data, &resp));  
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

> 这段代码是一个用于修改NX部件表达式的对话框的二次开发代码。其功能包括：
>
> 1. 创建一个对话框，其中包含一个单选列表，用于选择要修改的表达式，以及一个实数滑块，用于设置表达式的值。
> 2. 单选列表激活时，会更新滑块的值和精度，以匹配选定的表达式。
> 3. 滑块值改变时，会实时修改表达式的值。若修改无效，则恢复到原始值。
> 4. 另有一个整数滑块，用于设置滑块的精度。
> 5. 点击确定按钮时，会取消高亮显示特征。
> 6. 点击返回按钮时，会撤销到初始状态。
> 7. 点击取消按钮时，会撤销所有修改。
> 8. 通过undo标记点实现撤销功能。
> 9. 使用回调函数处理各种用户交互。
> 10. 主要使用了UF_MODL、UF_STYLER、UF_UNDO等NX API。
>
> 总的来说，这段代码实现了在对话框中修改NX部件表达式的功能，包括选择、修改和撤销等操作，提供了友好的用户界面。
>
