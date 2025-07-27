### 【1877】reorder features 重新排列特征

#### 代码

```cpp
    /*HEAD REORDER_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static int select_features(char *prompt, tag_t **feats)  
    {  
        int  
            cnt,  
            resp;  
        UF_CALL(UF_UI_select_feature(prompt, UF_UI_FEAT_SEL_TYPE_BROWSEABLE, &cnt,  
            feats, &resp));  
        if (resp == UF_UI_OK) return cnt;  
        else return 0;  
    }  
    /* qq3123197280 */  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int           
            ii;  
        uf_list_p_t   
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    /* qq3123197280 */  
    static logical ask_before_or_after( char *title, char *message )  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Before", NULL, "After", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        int  
            before = 1,  
            after = 2,  
            which_order = 0,  
            n_feats = 0;  
        tag_t  
            pivot = NULL_TAG,  
            part = NULL_TAG,  
            *feats = NULL;  
        uf_list_p_t  
            feat_list;  
        if((part=UF_PART_ask_display_part()) == NULL_TAG ) return;  
        if((pivot=select_a_feature("Pivot Feature:"))!= NULL_TAG)  
        {  
            n_feats = select_features("Feats to reorder:", &feats);  
            if( ask_before_or_after("Before or After?", "Choose:") == TRUE )  
            {  
                which_order = before;  
            }   
            else   
            {  
                which_order = after;  
            }  
            feat_list = make_a_list( n_feats, feats );  
            UF_CALL( UF_MODL_reorder_feature( pivot, feat_list, which_order));  
        }  
        if (n_feats > 0) UF_free(feats);  
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

> 这段代码是用于NX的二次开发，其主要功能是对特征进行重新排序。
>
> 主要功能包括：
>
> 1. 选择特征：select_a_feature函数用于选择一个特征作为pivot，而select_features函数用于选择要重新排序的特征列表。
> 2. 创建列表：make_a_list函数用于将特征数组转换成NX的列表。
> 3. 询问排序顺序：ask_before_or_after函数用于弹出一个对话框，询问用户是要将特征排在pivot之前还是之后。
> 4. 执行重新排序：在do_it函数中，首先选择要重新排序的特征和pivot特征，然后询问排序顺序，最后调用UF_MODL_reorder_feature函数进行重新排序。
> 5. 错误处理：通过UF_CALL宏进行NX函数调用，并在出错时报告错误。
> 6. 初始化和终止：在ufusr函数中，进行NX的初始化和终止。
> 7. 卸载处理：ufusr_ask_unload函数用于在卸载时立即卸载。
>
> 总体来说，这段代码实现了NX特征重新排序的功能，并提供了友好的用户交互。
>
