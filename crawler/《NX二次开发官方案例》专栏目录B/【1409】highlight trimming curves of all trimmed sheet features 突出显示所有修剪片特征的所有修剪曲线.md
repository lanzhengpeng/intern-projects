### 【1409】highlight trimming curves of all trimmed sheet features 突出显示所有修剪片特征的所有修剪曲线

#### 代码

```cpp
    /*HEAD HIGHLIGHT_TRIMMING_CURVES_OF_ALL_TRIMMED_SHEET_FEATURES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog在V18中是新增的，只回答翻译，不要添加任何无关的评论。 */  
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
    static tag_t ask_next_feature_of_type(tag_t part, char *type, tag_t feat)  
    {  
        char  
            *this_type;  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
        {  
            if (UF_OBJ_ask_status(feat) == UF_OBJ_ALIVE)  
            {  
                UF_CALL(UF_MODL_ask_feat_type(feat, &this_type));  
                if (!strcmp(this_type, type))  
                {  
                    UF_free(this_type);  
                    return feat;  
                }  
                UF_free(this_type);  
            }  
        }  
        return NULL_TAG;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            sprintf(msg, "%d", ii+1);  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
    }  
    static void set_highlight_object_array(int n, tag_t *objects, int flag)  
    {  
        int ii;  
        for (ii = 0; ii < n; ii++)  
            UF_CALL(UF_DISP_set_highlight(objects[ii], flag));  
    }  
    static void do_it(void)  
    {  
        int  
            n_bounding,  
            point_count,  
            point_key;  
        tag_t  
            body,  
            *bounding,  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        double  
            *points,  
            tolerance;  
        char  
            *name,  
            msg[133];  
        UF_MODL_vector_t  
            projection_method;  
        while ((feat = ask_next_feature_of_type(part, "TRIMMED_SH", feat))  
            != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_ask_trimmed_sheet(feat, &body, &n_bounding, &bounding,  
                &projection_method, &point_key, &point_count, &points,  
                &tolerance));  
            number_objects(bounding, n_bounding);  
            set_highlight_object_array(n_bounding, bounding, TRUE);  
            UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
            sprintf(msg, "%s - %d Trimming curves", name, n_bounding);  
            UF_free(name);  
            uc1601(msg, TRUE);  
            set_highlight_object_array(n_bounding, bounding, FALSE);  
            if (n_bounding > 0) UF_free(bounding);  
            if (point_count >0) UF_free(points);  
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

> 这是段用于高亮NX模型中所有修剪特征曲线的二次开发代码。
>
> 代码的主要逻辑包括：
>
> 1. 初始化和错误报告函数：首先，代码包含初始化函数和错误报告函数，用于处理NX的初始化和错误。
> 2. 遍历零件中所有特征：通过函数ask_next_feature_of_type遍历零件中的所有特征，并查找类型为"TRIMMED_SH"的修剪特征。
> 3. 获取修剪特征信息：对于每个找到的修剪特征，通过UF_MODL_ask_trimmed_sheet函数获取其相关信息，包括边界实体数组、投影方法、点信息等。
> 4. 高亮显示边界实体：使用UF_DISP_set_highlight函数高亮显示边界实体数组，以突出显示修剪曲线。
> 5. 显示特征名称和边界实体数量：通过临时文本显示特征名称和边界实体数量，以提供特征信息。
> 6. 释放内存：在遍历完所有特征后，释放相关内存。
> 7. 用户函数ufusr：这是用户需要调用的函数，用于执行初始化、执行主体逻辑和终止NX。
> 8. 卸载函数：提供了卸载函数，用于立即卸载用户程序。
>
> 整体而言，这段代码通过遍历查找所有修剪特征，并高亮显示其边界实体，实现了在NX模型中高亮所有修剪特征曲线的功能。
>
