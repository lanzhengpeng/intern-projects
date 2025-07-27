### 【2457】return arc radius to kf 返回弧半径给知识融合

#### 代码

```cpp
    /*HEAD RETURN_ARC_RADIUS_TO_KF CCC UFUN */  
    /*  This program demonstrates using UG/Open API to get the radius of an arc  
        back to Knowledge Fusion.  This example is for demonstration purposes  
        only, since you would normally use the "radius" attribute of the ug_arc  
        class, or the ug_curve_askCurvature function to obtain this  
        information.  
        The corresponding dfa file defining the KF function would look like  
        this:  
        #! UG/KF 18.0  
        #+  
            This function will return the radius of an arc  
            Input :  a HostPointer to an arc  
            Return:  number - the radius  
        #-  
        Defun: get_radiusRefObject (Any $object1,  
                       String ($libname, "/path/to/this.sl"),  
                       String ($funcname, "get_radiusRefObject") )  
        @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } Number ;  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_kf.h>  
    #include <uf_error_bases.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的函数。 */  
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
    /* qq3123197280 */  
    extern DllExport void get_radiusRefObject(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        int  
            edge_type,  
            subtype,  
            type;  
        tag_t  
            object;  
        UF_CURVE_arc_t  
            arc_data;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        if (UF_CALL(UF_KF_ask_tag(data[0], &object)) ||  
            (object == NULL_TAG))  
        {  
            UF_CALL(UF_KF_make_error(UF_err_bad_parameter_number_1, rv));  
            UF_terminate();  
            return;  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        if (type != UF_circle_type)  
        {  
            if ((type != UF_solid_type) ||  
                (subtype != UF_solid_edge_subtype) ||  
                UF_CALL(UF_MODL_ask_edge_type(object, &edge_type)) ||  
                (edge_type != UF_MODL_CIRCULAR_EDGE))  
            {  
                UF_CALL(UF_KF_make_error(UF_err_bad_parameter_number_1, rv));  
                UF_terminate();  
                return;  
            }  
        }  
        if (!UF_CALL(UF_CURVE_ask_arc_data(object, &arc_data)))  
            UF_CALL(UF_KF_make_number(arc_data.radius, rv));  
        else  
            UF_CALL(UF_KF_make_error(UF_err_operation_aborted, rv));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印出函数调用失败时的错误代码和错误消息。
> 2. 实现了一个名为get_radiusRefObject的函数，该函数接收一个NX对象句柄作为输入参数，并返回该对象的半径。函数首先判断输入对象是否为圆或圆弧，然后调用UF_CURVE_ask_arc_data函数获取圆弧数据，并从圆弧数据中获取半径值。
> 3. 定义了一个立即卸载的函数ufusr_ask_unload，用于在关闭NX时卸载二次开发库。
> 4. get_radiusRefObject函数使用了NX的开放接口，通过函数指针调用用户自定义函数，实现了获取圆弧半径的功能。
> 5. 代码中还包含了注释，说明了函数的参数、返回值、功能等，以及NX版本的相关说明。
>
> 综上所述，这段代码通过NX的开放接口，实现了在Knowledge Fusion中获取圆弧半径的功能，为NX的二次开发提供了一个示例。
>
