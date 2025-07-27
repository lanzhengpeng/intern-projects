### 【1920】report all flat pattern curve ends 报告所有展平图案曲线端点

#### 代码

```cpp
    /*HEAD REPORT_ALL_FLAT_PATTERN_CURVE_ENDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_smd.h>  
    #include <uf_group.h>  
    #include <uf_eval.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数，用于打印系统日志消息。 */  
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
    static tag_t ask_next_visible_solid(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (!UF_CALL(UF_VIEW_cycle_objects(NULL_TAG, UF_VIEW_VISIBLE_OBJECTS,  
                &body)) && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if ((type == UF_solid_type) && (subtype == UF_solid_body_subtype))  
                return body;  
        }  
        return NULL_TAG;  
    }  
    #define WRITE3F(X)  (write_double_array_to_listing_window(#X, X, 1, 3))  
    static void write_double_array_to_listing_window(char *name, double *array,  
            int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        char  
            blanks[MAX_LINE_SIZE+1],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        memset(blanks, ' ', strlen(name) + 3);  
        blanks[strlen(name) + 3] = '\0';  
        for (ii = 0; ii < rows; ii++)  
        {  
            if (ii == 0) sprintf(msg, "%s = ", name);  
            else sprintf(msg, "%s", blanks);  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window(msg));  
            for (jj = 0; jj < cols; jj++)  
            {  
                sprintf(msg, "%f", array[kk++]);  
                UF_CALL(UF_UI_write_listing_window(msg));  
                if (jj < cols - 1)  
                {  
                    sprintf(msg, ", ");  
                    UF_CALL(UF_UI_write_listing_window(msg));  
                }  
            }  
            if (cols > 0) UF_CALL(UF_UI_write_listing_window("\n"));  
        }  
    }  
    static void ask_curve_ends(tag_t curve, double start[3], double end[3])  
    {  
        double  
            limits[2];  
        UF_EVAL_p_t  
            evaluator;  
        UF_CALL(UF_EVAL_initialize(curve, &evaluator));  
        UF_CALL(UF_EVAL_ask_limits(evaluator, limits));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[0], start, NULL));  
        UF_CALL(UF_EVAL_evaluate(evaluator, 0, limits[1], end, NULL));  
        UF_CALL(UF_EVAL_free(evaluator));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_members;  
        tag_t  
            body = NULL_TAG,  
            flat_pattern,  
            *members;  
        double  
            end[3],  
            start[3];  
        if (UF_CALL(UF_SMD_initialize())) return;  
        while ((body = ask_next_visible_solid(body)) != NULL_TAG)  
        {  
            UF_CALL(UF_SMD_ask_flat_pattern(body, &flat_pattern));  
            if (flat_pattern == NULL_TAG) continue;  
            UF_CALL(UF_GROUP_ask_group_data(flat_pattern, &members, &n_members));  
            for (ii = 0; ii < n_members; ii++)  
            {  
                ask_curve_ends(members[ii], start, end);  
                WRITE3F(start);  
                WRITE3F(end);  
            }  
            if (n_members > 0) UF_free(members);  
        }  
        UF_CALL(UF_SMD_terminate());  
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

> 这段代码是NX Open C++的一个示例程序，主要功能是遍历当前活动部件中的所有可见实体，并报告每个实体的直纹面展开。具体步骤如下：
>
> 1. 初始化UF模块，包括UF、UI、SMD、GROUP、EVAL、VIEW和OBJ。
> 2. 使用ask_next_visible_solid函数循环遍历当前活动部件中的所有可见实体，直到没有更多实体。
> 3. 对于每个实体，使用UF_SMD_ask_flat_pattern函数查询其直纹面展开，如果存在，则继续。
> 4. 使用UF_GROUP_ask_group_data函数获取直纹面展开中的所有成员（通常是曲线）。
> 5. 遍历所有成员，使用ask_curve_ends函数查询每个曲线的起点和终点，并使用write_double_array_to_listing_window函数将起点和终点坐标输出到列表窗口。
> 6. 释放分配的内存，并终止UF模块。
> 7. 在ufusr函数中调用主函数do_it。
> 8. 提供ufusr_ask_unload函数以立即卸载该程序。
>
> 总的来说，这段代码提供了一个查询和报告NX模型中所有直纹面展开的示例，具有一定的参考价值。
>
