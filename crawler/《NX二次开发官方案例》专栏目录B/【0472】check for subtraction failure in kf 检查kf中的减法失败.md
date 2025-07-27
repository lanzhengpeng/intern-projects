### 【0472】check for subtraction failure in kf 检查kf中的减法失败

#### 代码

```cpp
    /*HEAD CHECK_FOR_SUBTRACTION_FAILURE_IN_KF CCC UFUN */  
    /*  This program demonstrates using UG/Open API to determine whether a  
        ug_boolean subtract operation will fail in Knowledge Fusion or not  
        and return the result back to KF.  
        The corresponding dfa file defining the KF function would look like  
        this:  
        #! UG/KF 19.0  
        #+  
            This function will try to perform a specified boolean operation  
            on the solids provided and return a boolean indicating whether it is  
            going to work or not to KF.  
            Input : two solid body or feature instances or hostpointers  
            Return: Boolean - True = The subtract operation will fail  
                              False = The subtract operation will NOT fail  
        #-  
        Defun: subtractFails?(  
               any $target_solid,  
               any $tool_solid,  
               String ($libname, "check_for_subtraction_failure_in_kf"),  
               String ($funcname, "test_subtract") )  
        @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } boolean ;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <ctype.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档，UF_print_syslog是V18版本新增的功能。

翻译如下：
注意：UF_print_syslog是V18版本新增的。 */  
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
    extern DllExport void test_subtract(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        int  
            ii,  
            n = 0,  
            subtype,  
            type;  
        tag_t  
            object[2],  
            *results;  
        UF_UNDO_mark_id_t  
            mark;  
        UF_KF_instance_p_t  
            instance;  
        UF_KF_type_t  
            data_type;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        UF_CALL(UF_KF_make_boolean(TRUE, rv));  
        for (ii = 0; ii < 2; ii++)  
        {  
            UF_CALL(UF_KF_ask_type(data[ii], &data_type));  
            switch (data_type)  
            {  
                case UF_KF_instance:  
                    if (UF_CALL(UF_KF_ask_instance(data[ii], &instance)) ||  
                        UF_CALL(UF_KF_ask_ug_object(instance, &object[ii])) ||  
                        (object[ii] == NULL_TAG)) return;  
                    break;  
                case UF_KF_tag:  
                    if (UF_CALL(UF_KF_ask_tag(data[ii], &object[ii])) ||  
                        (object[ii] == NULL_TAG)) return;  
                    break;  
                default:  
                    return;  
                    break;  
            }  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object[ii], &type, &subtype));  
            if ((type == UF_feature_type) &&  
                UF_CALL(UF_MODL_ask_feat_body(object[ii], &object[ii]))) return;  
        }  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, NULL, &mark));  
        if (!UF_MODL_subtract_bodies(object[0], object[1], &n, &results) &&  
            (n == 1)) UF_CALL(UF_KF_make_boolean(FALSE, rv));  
        if (n > 0) UF_free(results);  
        UF_CALL(UF_UNDO_undo_to_mark(mark, NULL));  
        UF_CALL(UF_UNDO_delete_mark(mark, NULL));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是检测两个实体进行布尔减操作是否会导致失败，并将结果返回给知识融合(Knowledge Fusion)。主要包含以下内容：
>
> 1. 函数定义：test_subtract函数接收两个实体的实例或指针作为输入，并返回布尔值，表示布尔减操作是否会失败。
> 2. 初始化NX：代码首先进行NX初始化，然后获取输入的两个实体的NX对象标签，判断其类型，并获取其对应的NX对象。
> 3. 标记操作：使用UF_UNDO_set_mark来标记当前状态，以备后续撤销操作。
> 4. 执行布尔减操作：尝试执行布尔减操作，如果失败，则返回布尔值FALSE。
> 5. 撤销操作：使用UF_UNDO_undo_to_mark撤销到标记状态，以确保NX状态不变。
> 6. 清理资源：释放相关资源，结束NX会话。
> 7. 错误处理：使用report_error函数来报告错误，并输出到系统日志和列表窗口。
> 8. 卸载处理：ufusr_ask_unload函数返回立即卸载，表示该动态库不需要延迟卸载。
>
> 总的来说，这段代码实现了检测两个实体进行布尔减操作是否会导致失败的功能，并将结果返回给知识融合，以供后续流程判断。
>
