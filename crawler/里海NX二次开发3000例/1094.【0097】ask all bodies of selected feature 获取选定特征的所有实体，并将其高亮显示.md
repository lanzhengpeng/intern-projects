### 【0097】ask all bodies of selected feature 获取选定特征的所有实体，并将其高亮显示

#### 代码

```cpp
    /*HEAD ASK_ALL_BODIES_OF_SELECTED_FEATURE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
    }  
    static int uf_modl_ask_feat_bodies( tag_t part, tag_t feature,   
                                                   uf_list_p_t bodies )  
    {  
        int  
            inx = 0,  
            body_count = 0,  
            feat_count = 0;  
        tag_t  
            body = NULL_TAG,  
            feat = NULL_TAG;  
        uf_list_p_t   
            feature_tag_list;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_body_feats ( body, &feature_tag_list ));  
            UF_CALL(UF_MODL_ask_list_count( feature_tag_list, &feat_count ));  
            for( inx = 0; inx < feat_count; inx++ )  
            {  
                UF_CALL(UF_MODL_ask_list_item(feature_tag_list, inx, &feat));   
                if(feat == feature)  
                {  
                     UF_CALL(UF_MODL_put_list_item(bodies, body));  
                }    
            }  
            if( feat_count > 0 )   
            {  
                UF_CALL( UF_MODL_delete_list ( & feature_tag_list ));  
            }  
        }  
        UF_CALL(UF_MODL_ask_list_count( bodies, &body_count ));  
        return( body_count );  
    }  
    static void do_it(void)  
    {  
        int  
            inx = 0,  
            num_bodies = 0;  
        tag_t  
            feat = NULL_TAG,  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            body_list;  
        if( NULL_TAG == part ) return;  
        while (( feat=select_a_feature("Feature: "))!= NULL_TAG)  
        {  
            UF_CALL(UF_MODL_create_list(&body_list));  
            num_bodies = uf_modl_ask_feat_bodies( part, feat, body_list );  
            WRITENZ( num_bodies );  
            for( inx = 0; inx < num_bodies; inx++ )  
            {  
                UF_CALL(UF_MODL_ask_list_item(body_list, inx, &body ));  
                UF_DISP_set_highlight(body, TRUE);  
            }  
            if( num_bodies > 0 )  
                uc1601( "Bodies belonging to selected Feature", TRUE );  
            for( inx = 0; inx < num_bodies; inx++ )  
            {  
                UF_CALL(UF_MODL_ask_list_item(body_list, inx, &body ));  
                UF_DISP_set_highlight(body, FALSE);  
            }  
            UF_CALL( UF_MODL_delete_list ( &body_list  ));  
        }  
    }  
    /* 里海 */  
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

> 根据代码注释和功能，这段NX二次开发代码实现了以下功能：
>
> 1. 提示用户选择特征：通过UF_UI_select_feature函数提示用户选择特征，并获取所选特征。
> 2. 查询所选特征关联的实体：通过自定义函数uf_modl_ask_feat_bodies，查询用户所选特征关联的实体，并将这些实体保存到一个列表中。
> 3. 高亮显示关联实体：使用UF_DISP_set_highlight函数将所选特征关联的实体高亮显示，以便用户查看。
> 4. 循环选择特征和显示：在循环中，用户可以继续选择特征，并查看其关联的实体。
> 5. 错误处理和日志记录：通过自定义函数report_error记录函数调用的错误代码和错误信息。
> 6. 初始化和结束NX：通过UF_initialize和UF_terminate函数进行NX的初始化和结束。
> 7. 卸载函数：通过自定义函数ufusr_ask_unload实现二次开发的卸载。
>
> 总体而言，这段代码实现了根据用户选择特征，高亮显示其关联实体的功能，方便用户直观地查看特征与实体的关系，提高了NX软件的易用性。
>
