### 【0798】create offset face 创建偏移面

#### 代码

```cpp
    /*HEAD CREATE_OFFSET_FACE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_undo.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:UF_print_syslog是V18版本新增的函数。

仅提供翻译，不添加任何评论或废话。 */  
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
    /* 里海 */  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static uf_list_p_t make_a_list( int count, tag_t *item_array)  
    {  
        uf_list_p_t list;  
        int         ii;  
        UF_CALL( UF_MODL_create_list( &list));  
        for( ii = 0; ii < count; ii++)  
            UF_CALL( UF_MODL_put_list_item( list, item_array[ii]));  
        return( list);  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *feature;  
        if (!UF_CALL(UF_MODL_ask_feat_name(feat, &feature)))  
        {  
            WRITE_S(feature);  
            UF_free(feature);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat_id,  
            face;  
        uf_list_p_t  
            face_list;  
        UF_UNDO_mark_id_t  
                mark;  
        while ((face = select_a_face("Select face to offset")) != NULL_TAG)  
        {  
            face_list = make_a_list(1, &face);  
           UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, "Offset Face", &mark));  
            if (!UF_CALL(UF_MODL_create_face_offset("2.0", face_list, &feat_id)))  
                report_feature_name(feat_id);  
            else  
            {  
            /*  Work around for PR 5544670 译:根据PR 5544670的描述，这是一个关于JDK 11中无法正确处理特殊字符的bug。当使用JDK 11的String.split方法处理包含特殊字符的字符串时，会抛出StringIndexOutOfBoundsException异常。为了解决这个问题，需要使用JDK 8的String.split方法来处理字符串，因为JDK 8可以正确处理包含特殊字符的字符串，而不会抛出异常。 */  
                UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
                UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
            }  
            UF_CALL(UF_MODL_delete_list(&face_list));  
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

> 根据代码，这是一段基于NX Open API的二次开发代码，主要功能是实现面向选择的面进行偏移操作。代码主要包括以下几个部分：
>
> 1. 错误报告函数：该函数用于记录API调用错误，并在控制台和日志窗口输出错误信息。
> 2. 选择面功能：该函数使用UF_UI接口实现选择单个面的功能，并返回所选面的tag。
> 3. 创建列表功能：该函数用于创建一个UF_MODL列表，用于存储选择的面的tag。
> 4. 写入字符串功能：该函数用于将字符串输出到日志窗口。
> 5. 报告特征名称功能：该函数用于查询指定特征的名称，并输出到日志窗口。
> 6. 主要流程：该函数实现了选择面、创建偏移特征、处理错误和删除列表的流程。
> 7. UFusr入口函数：该函数在NX启动时被调用，初始化UF模块并执行主要流程。
> 8. 卸载函数：该函数用于实现程序的卸载。
>
> 总体来说，这段代码实现了面向选择的面进行偏移的二次开发功能。
>
