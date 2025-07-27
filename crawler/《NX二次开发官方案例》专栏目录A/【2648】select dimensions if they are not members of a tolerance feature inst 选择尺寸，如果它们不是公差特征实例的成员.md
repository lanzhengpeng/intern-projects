### 【2648】select dimensions if they are not members of a tolerance feature inst 选择尺寸，如果它们不是公差特征实例的成员

#### 代码

```cpp
    /*HEAD SELECT_DIMENSIONS_IF_THEY_ARE_NOT_MEMBERS_OF_A_TOLERANCE_FEATURE_INST CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_part.h>  
    #include <uf_ui.h>  
    #include <uf_gdt.h>  
    #include <uf_cfi.h>  
    #include <uf_obj.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog 是在 V18 版本中新增的功能。 */  
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
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /********************************************************************/  
    /* note that these are scoped globally on purpose  - a rare occurrence 里海译:这些是有意全局范围的 - 这是一个罕见的情况 */  
        int  
            dim_count = 0;  
        tag_t  
            global_dimension[1000];   
    /********************************************************************/  
    /* qq3123197280 */  
    static int filter_tol_feat_dims( tag_t object, int type[3],   
                                    void *data, UF_UI_selection_p_t select)  
    {  
        int  
            inx = 0;  
        logical  
            is_tol_feat_dim = FALSE;  
        for( inx = 0; inx < dim_count; inx++ )  
        {  
            if( object == global_dimension[inx] ) is_tol_feat_dim = TRUE;  
            if ( is_tol_feat_dim ) return (UF_UI_SEL_REJECT);  
        }  
        /* If it hasn't returned by now, it fell through the sieve 里海译:如果到现在还没回来，那就是石沉大海了。 */  
        return (UF_UI_SEL_ACCEPT);  
    }  
    /* qq3123197280 */  
    static int mask_for_non_tol_feat_dimensions( UF_UI_selection_p_t select,   
                                                                 void *type )  
    {  
        UF_UI_mask_t  
            mask = { UF_dimension_type, UF_all_subtype, 0 };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_tol_feat_dims, NULL, type))  
           )  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    /* qq3123197280 */  
    static tag_t select_a_dimension(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a dimension", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_non_tol_feat_dimensions,   
            NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void initialize_global_tags()  
    {  
        int  
            inx = 0;  
        for (inx = 0; inx < 1000; inx++ ) global_dimension[inx] = NULL_TAG;  
    }  
    /* qq3123197280 */  
    static tag_t ask_next_of_type(tag_t part, int type, tag_t object)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, type, &object));  
        return (object);  
    }  
    static void get_tol_feat_dimension_tags(void)  
    {  
        int  
            inx = 0,  
            jnx = 0,  
            type,  
            subtype;  
        tag_t  
            tol_feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_GDT_annotation_tags_t   
            tags;  
        initialize_global_tags();      
        while ((tol_feat = ask_next_of_type( part,   
                      UF_tol_feature_instance_type, tol_feat )) != NULL_TAG )   
        {  
            if( tol_feat == NULL_TAG ) break;  
            UF_CALL(UF_OBJ_ask_type_and_subtype(tol_feat, &type, &subtype));  
            if( subtype == 0 )   
            {                   
                UF_CALL( UF_GDT_ask_annotation_tags ( tol_feat, &tags ));  
                for( inx = 0; inx < tags.num_dimension_tags; inx++ )  
                {  
                    global_dimension[jnx] = tags.dimension_tags[inx];  
                    jnx++;  
                }  
                UF_GDT_free( UF_GDT_ANNOTATION_TAGS_TYPE, &tags );  
            }   
        }  
        dim_count = jnx;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            my_dim = NULL_TAG;  
        get_tol_feat_dimension_tags();  
        while( (my_dim = select_a_dimension("Select dimension:")) != NULL_TAG)  
        {  
            WRITE_D( my_dim );  
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

> 这段代码是一个NX的二次开发UF程序，主要用于选择非公差特征尺寸。
>
> 主要功能包括：
>
> 1. 定义了一个全局数组global_dimension，用于存储公差特征中的尺寸标签。
> 2. get_tol_feat_dimension_tags函数用于遍历当前显示部件中的所有公差特征实例，并提取其中的尺寸标签，存储到global_dimension数组中。
> 3. filter_tol_feat_dims函数用于过滤选择，拒绝公差特征中的尺寸。
> 4. mask_for_non_tol_feat_dimensions函数用于设置选择过滤器，只允许选择非公差特征的尺寸。
> 5. select_a_dimension函数用于让用户选择一个尺寸，并返回尺寸标签。
> 6. do_it函数循环调用select_a_dimension，让用户选择尺寸，并打印尺寸标签。
> 7. ufusr函数是程序的入口函数，初始化NX环境，调用do_it执行功能，然后终止NX环境。
> 8. ufusr_ask_unload函数用于处理程序的卸载。
>
> 总体来说，该代码通过获取公差特征中的尺寸标签，并设置选择过滤器，实现了只选择非公差特征尺寸的功能。
>
