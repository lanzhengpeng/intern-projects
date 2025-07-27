### 【2102】report facepair feature type 报告面组特征类型

#### 代码

```cpp
    /*HEAD REPORT_FACEPAIR_FEATURE_TYPE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据提供的信息，UF_print_syslog是V18版本中的新功能。 */  
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
    static tag_t ask_next_feature(tag_t part, tag_t feat)  
    {  
        while (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_feature_type, &feat))  
            && (feat != NULL_TAG))  
            if (UF_OBJ_ask_status(feat) == 3) return feat;  
        return NULL_TAG;  
    }  
    static void report_facepair_type(tag_t feat)  
    {  
        int  
            ii,  
            facepair_type,  
            jj;  
        char  
            msg[133],  
            *name = { "Name unknown" },  
            *type = { "Type unknown" };  
        tag_t  
            midsrf_feature_obj_id,  
            defining_face_1,  
            defining_face_2,  
            user_selected_midsurface,  
            midsurface_sheet_body;  
        uf_list_p_t  
            side_1,  
            side_2;  
        ii = UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
        jj = UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        if ((strcmp(type, "FACEPAIR_DEF")) == 0)  
        {  
            UF_CALL(UF_MODL_ask_facepair_parms(feat, &facepair_type,  
                &midsrf_feature_obj_id, &defining_face_1, &defining_face_2,  
                &user_selected_midsurface, &side_1, &side_2,  
                &midsurface_sheet_body));  
            sprintf(msg, "%d = %s (%s) facepair type = %d\n", feat, name, type,  
                facepair_type);  
            UF_CALL(UF_UI_write_listing_window(msg));  
            UF_CALL(UF_MODL_delete_list(&side_1));  
            UF_CALL(UF_MODL_delete_list(&side_2));  
        }  
        if (ii == 0) UF_free(name);  
        if (jj == 0) UF_free(type);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_CALL(UF_UI_open_listing_window());  
        while ((feat = ask_next_feature(part, feat)) != NULL_TAG)  
            report_facepair_type(feat);  
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

> 根据提供的NX二次开发代码，我将对该代码进行简要介绍：
>
> 1. 代码目的：该代码用于遍历当前显示的NX部件中的所有特征，并报告每个面的配对类型。
> 2. 主要函数：do_it：遍历当前部件中的所有特征，并调用report_facepair_type函数报告每个特征的面配对类型。report_facepair_type：获取特征名称、类型，并判断是否为面配对特征。如果是，则报告其面配对类型。ask_next_feature：遍历部件中的下一个特征，并返回其标签。report_error：用于报告函数调用失败的错误信息。ufusr：二次开发入口函数，初始化后调用do_it函数，最后终止。
> 3. do_it：遍历当前部件中的所有特征，并调用report_facepair_type函数报告每个特征的面配对类型。
> 4. report_facepair_type：获取特征名称、类型，并判断是否为面配对特征。如果是，则报告其面配对类型。
> 5. ask_next_feature：遍历部件中的下一个特征，并返回其标签。
> 6. report_error：用于报告函数调用失败的错误信息。
> 7. ufusr：二次开发入口函数，初始化后调用do_it函数，最后终止。
> 8. 主要功能：使用UF_OBJ函数遍历部件中的特征。使用UF_MODL函数获取特征名称、类型、面配对参数等。使用UF_UI函数在列表窗口中打印信息。使用错误处理机制，在函数调用失败时报告错误。
> 9. 使用UF_OBJ函数遍历部件中的特征。
> 10. 使用UF_MODL函数获取特征名称、类型、面配对参数等。
> 11. 使用UF_UI函数在列表窗口中打印信息。
> 12. 使用错误处理机制，在函数调用失败时报告错误。
> 13. 注意事项：在NX 18及以上版本中使用UF_print_syslog输出日志。释放使用UF_free分配的内存。在特征遍历结束后，删除使用UF_MODL_delete_list创建的列表。
> 14. 在NX 18及以上版本中使用UF_print_syslog输出日志。
> 15. 释放使用UF_free分配的内存。
> 16. 在特征遍历结束后，删除使用UF_MODL_delete_list创建的列表。
> 17. 代码编写风格：代码遵循了NX二次开发的一般规范，具有清晰的逻辑结构，注释丰富，方便他人阅读和理解。
>
> 以上是对代码的简要介绍，如有需要补充或修改的地方，欢迎指正。
>
