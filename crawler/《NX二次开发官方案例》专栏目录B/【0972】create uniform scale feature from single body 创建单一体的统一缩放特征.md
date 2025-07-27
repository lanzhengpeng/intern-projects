### 【0972】create uniform scale feature from single body 创建单一体的统一缩放特征

#### 代码

```cpp
    /*HEAD CREATE_UNIFORM_SCALE_FEATURE_FROM_SINGLE_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog 是 V18 新增的功能。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            body = NULL_TAG,  
            scale_feat = NULL_TAG;  
        double  
            scale = 1.;  
        char  
            scale_factor[132] = { "" };  
        while( (( body = select_a_body( "Select a Body:" )) != NULL_TAG ) &&  
               ( prompt_for_a_number( "New Scale:", "Scale:", &scale ) == TRUE )  
             )   
        {  
            sprintf( scale_factor, "%f", scale );  
            UF_CALL( UF_MODL_create_uniform_scale ( body, UF_CSYS_WORK_COORDS,   
                     scale_factor, &scale_feat ));  
            WRITE( "New tag for scale feature \n" );  
            WRITE_D( scale_feat );  
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

> 这段NX Open C++代码用于创建一个均匀缩放特征，其功能是从一个实体创建均匀缩放特征。
>
> 主要功能包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义整数写入列表窗口函数write_integer_to_listing_window。
> 3. 定义用于选择实体的选择掩码函数mask_for_bodies。
> 4. 定义用于选择实体的函数select_a_body。
> 5. 定义用于提示用户输入数值的函数prompt_for_a_number。
> 6. 定义主函数do_it，其中循环选择实体并提示用户输入缩放比例，然后调用UF_MODL_create_uniform_scale函数创建均匀缩放特征。
> 7. 定义NX Open的初始化和终止函数ufusr。
> 8. 定义卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码实现了从单个实体创建均匀缩放特征的功能，提供了用户交互界面来选择实体和输入缩放比例，并使用了NX Open提供的API函数来创建特征。
>
