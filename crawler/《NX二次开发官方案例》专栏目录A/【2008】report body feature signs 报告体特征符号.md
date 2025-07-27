### 【2008】report body feature signs 报告体特征符号

#### 代码

```cpp
    /*HEAD REPORT_BODY_FEATURE_SIGNS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog是V18版本新增的。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_body_feats(tag_t body, tag_p_t *body_feats)  
    {  
        int  
            n;  
        uf_list_p_t  
            feat_list;  
        UF_CALL(UF_MODL_ask_body_feats(body, &feat_list));  
        n = make_an_array(&feat_list, body_feats);  
        return n;  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *string,  
            msg[300] = { "" };  
        UF_CALL(UF_MODL_ask_feat_name(feat, &string));  
        sprintf( msg, "\n%d = %s\n", feat, string );  
        WRITE( msg );  
        UF_free(string);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            body = NULL_TAG,  
            *feats,  
            part = UF_PART_ask_display_part();  
        UF_FEATURE_SIGN  
            boolean_status,  
            sign;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n = ask_body_feats(body, &feats);  
            for (ii = 0; ii < n; ii++)  
            {  
                report_feature_name(feats[ii]);  
                UF_CALL(UF_MODL_ask_feature_boolean(feats[ii], &boolean_status));  
                WRITE_D(boolean_status);  
                UF_CALL(UF_MODL_ask_feature_sign(feats[ii], &sign));  
                WRITE_D(sign);  
            }  
            UF_free(feats);  
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

> 这是段NX二次开发代码，主要用于遍历零件中的实体，并报告每个实体的特征名称、布尔状态和符号状态。以下是该代码的主要功能和流程：
>
> 1. 定义了一些宏和错误报告函数，用于在出现错误时输出错误信息。
> 2. 定义了一个函数ask_next_body，用于遍历零件中的实体，返回下一个实体。
> 3. 定义了一个函数allocate_memory，用于分配内存。
> 4. 定义了一个函数make_an_array，用于将列表转换为数组。
> 5. 定义了一个函数ask_body_feats，用于查询实体中的所有特征，并返回特征数组。
> 6. 定义了一个函数report_feature_name，用于报告特征的名称。
> 7. 定义了一个函数do_it，用于遍历零件中的实体，报告每个实体的特征名称、布尔状态和符号状态。
> 8. 定义了ufusr函数，作为程序的入口，初始化NX环境，调用do_it函数，然后终止NX环境。
> 9. 定义了ufusr_ask_unload函数，用于在程序卸载时立即释放内存。
>
> 总的来说，这段代码实现了遍历零件实体，查询每个实体的特征信息，并报告每个特征的名称、布尔状态和符号状态的功能。
>
