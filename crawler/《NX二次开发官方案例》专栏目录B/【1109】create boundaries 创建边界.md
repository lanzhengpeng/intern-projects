### 【1109】create boundaries 创建边界

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_bound.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    /* 里海 */  
    static int mask_for_curves(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask[5] = { { UF_line_type, 0, 0 },  
                        { UF_circle_type, 0, 0 },  
                        { UF_conic_type, UF_all_subtype, 0 },  
                        { UF_spline_type, 0, 0 },  
                        { UF_point_type, 0, 0 } };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 5, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static int select_curves(char *prompt, tag_t **curves)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        UF_CALL(UF_UI_select_with_class_dialog("Select curves", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_curves, NULL, &resp, &cnt, curves));  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight((*curves)[ii], FALSE));  
        return cnt;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n;  
        tag_t  
            boundary,  
            *curves;  
        UF_BOUND_tolerance_t  
            tol = { UF_BOUND_NOT_SPECIFIED, 0.0, 0.0 };  
        UF_BOUND_object_t  
            *objects;  
        if ((n = select_curves("Select curves for OPEN boundary", &curves)) > 0)  
        {  
            UF_CALL(allocate_memory((n * sizeof(UF_BOUND_object_t)), (void **)&objects));  
            for (ii = 0; ii < n; ii++)  
            {  
                objects[ii].object_tag = curves[ii];  
                objects[ii].on_tangent_to_flag = UF_BOUND_ON;  
            }  
            UF_CALL(UF_BOUND_create_boundary(UF_BOUND_OPEN, NULL_TAG, tol, n,  
                objects, &boundary));  
            UF_CALL(UF_OBJ_set_name(boundary, "OPEN"));  
            UF_free(curves);  
            UF_free(objects);  
        }  
        if ((n = select_curves("Select curves for CLOSED boundary", &curves)) > 0)  
        {  
            UF_CALL(allocate_memory((n * sizeof(UF_BOUND_object_t)), (void **)&objects));  
            for (ii = 0; ii < n; ii++)  
            {  
                objects[ii].object_tag = curves[ii];  
                objects[ii].on_tangent_to_flag = UF_BOUND_ON;  
            }  
            UF_CALL(UF_BOUND_create_boundary(UF_BOUND_CLOSED, NULL_TAG, tol, n,  
                objects, &boundary));  
            UF_CALL(UF_OBJ_set_name(boundary, "CLOSED"));  
            UF_free(curves);  
            UF_free(objects);  
        }  
    }  
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

> 这是NX二次开发的UF程序，其主要功能是创建开环和闭环边界。
>
> 1. 函数介绍：ECHO：用于输出信息到日志窗口。UF_CALL：用于调用UF函数，并检查返回码。report_error：用于报告错误信息。mask_for_curves：设置选择过滤，仅选择线、圆、二次曲线、样条线和点。select_curves：提示用户选择曲线，并返回选择的曲线数组。allocate_memory：申请内存。do_it：主函数，用于创建开环和闭环边界。ufusr：程序入口函数。ufusr_ask_unload：询问卸载函数。
> 2. ECHO：用于输出信息到日志窗口。
> 3. UF_CALL：用于调用UF函数，并检查返回码。
> 4. report_error：用于报告错误信息。
> 5. mask_for_curves：设置选择过滤，仅选择线、圆、二次曲线、样条线和点。
> 6. select_curves：提示用户选择曲线，并返回选择的曲线数组。
> 7. allocate_memory：申请内存。
> 8. do_it：主函数，用于创建开环和闭环边界。
> 9. ufusr：程序入口函数。
> 10. ufusr_ask_unload：询问卸载函数。
> 11. 程序流程：初始化UF环境。调用do_it函数。选择曲线创建开环边界。再次选择曲线创建闭环边界。终止UF环境。
> 12. 初始化UF环境。
> 13. 调用do_it函数。选择曲线创建开环边界。再次选择曲线创建闭环边界。
> 14. 选择曲线创建开环边界。
> 15. 再次选择曲线创建闭环边界。
> 16. 终止UF环境。
> 17. 主要逻辑：通过select_curves选择曲线。根据曲线数组，申请内存并构建边界对象数组。调用UF_BOUND_create_boundary创建边界，参数指定开环或闭环。设置边界名称。释放内存。
> 18. 通过select_curves选择曲线。
> 19. 根据曲线数组，申请内存并构建边界对象数组。
> 20. 调用UF_BOUND_create_boundary创建边界，参数指定开环或闭环。
> 21. 设置边界名称。
> 22. 释放内存。
> 23. 注意事项：使用UF_CALL和report_error进行错误处理。使用mask_for_curves过滤选择。使用allocate_memory申请内存。在创建边界后释放内存。
> 24. 使用UF_CALL和report_error进行错误处理。
> 25. 使用mask_for_curves过滤选择。
> 26. 使用allocate_memory申请内存。
> 27. 在创建边界后释放内存。
> 28. 优化建议：可以考虑使用结构体封装边界信息，简化代码。可以考虑增加错误处理，例如边界创建失败的情况。可以考虑增加用户交互，例如提示用户确认创建的边界。
> 29. 可以考虑使用结构体封装边界信息，简化代码。
> 30. 可以考虑增加错误处理，例如边界创建失败的情况。
> 31. 可以考虑增加用户交互，例如提示用户确认创建的边界。
>
> 总体来说，这段代码实现了创建开环和闭环边界的功能，但仍有改进空间，例如增加错误处理和用户交互。
>
