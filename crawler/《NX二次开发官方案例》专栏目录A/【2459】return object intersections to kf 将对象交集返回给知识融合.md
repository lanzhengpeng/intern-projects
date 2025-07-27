### 【2459】return object intersections to kf 将对象交集返回给知识融合

#### 代码

```cpp
    /*HEAD RETURN_OBJECT_INTERSECTIONS_TO_KF CCC UFUN */  
    /*  This program demonstrates using UG/Open API to get the intersection of  
        two Knowledge Fusion objects back to Knowledge Fusion.  
        The corresponding dfa file defining the KF function would look like  
        this:  
        #! UG/KF 18.0  
        #+  
            This function will return the intersections between two objects  
            Input : two object instances  
            Return: list - intersection points or instances  
        #-  
        Defun: get_intersectionTwoObjects (instance $object1, instance $object2,  
                       String ($libname, "/path/to/this.sl"),  
                       String ($funcname, "get_intersectionTwoObjects") )  
        @{ Cfunc("UF_KF_invoke_user_function"; "libufun"); } list ;  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是V18版本中新增的函数。 */  
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
    extern DllExport void get_intersectionTwoObjects(  
        UF_KF_value_t *data,  
        int num,  
        UF_KF_value_t *rv)  
    {  
        int  
            ii,  
            n,  
            subtype,  
            type;  
        tag_t  
            *eids,  
            object[2];  
        double  
            tol;  
        UF_KF_value_p_t  
            value;  
        UF_KF_list_p_t  
            results = NULL;  
        UF_KF_instance_p_t  
            instance;  
        UF_MODL_intersect_info_p_t  
            *intersections;  
        *rv = NULL;  
        if (UF_CALL(UF_initialize())) return;  
        for (ii = 0; ii < 2; ii++)  
        {  
            if (UF_CALL(UF_KF_ask_instance(data[ii], &instance)) ||  
                UF_CALL(UF_KF_ask_ug_object(instance, &object[ii])) ||  
                (object[ii] == NULL_TAG))  
            {  
                UF_CALL(UF_KF_make_error(ERROR_UF_base+4+ii, &value));  
                UF_CALL(UF_KF_list_push(results, value, &results));  
                UF_CALL(UF_KF_make_list(results, rv));  
                return;  
            }  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object[ii], &type, &subtype));  
            if (type == UF_feature_type)  
            {  
                if (UF_CALL(UF_MODL_ask_feat_object(object[ii], &n, &eids)) ||  
                    (n != 1))  
                {  
                    UF_CALL(UF_KF_make_error(ERROR_UF_base+4+ii, &value));  
                    UF_CALL(UF_KF_list_push(results, value, &results));  
                    UF_CALL(UF_KF_make_list(results, rv));  
                    if (n > 0) UF_free(eids);  
                    return;  
                }  
                object[ii] = eids[0];  
                UF_free(eids);  
            }  
        }  
        UF_MODL_ask_distance_tolerance(&tol);  
        if (UF_CALL(UF_MODL_intersect_objects(object[0], object[1], tol, &n,  
            &intersections)))  
        {  
            UF_CALL(UF_KF_make_error(UF_err_operation_aborted, &value));  
            UF_CALL(UF_KF_list_push(results, value, &results));  
            UF_CALL(UF_KF_make_list(results, rv));  
            return;  
        }  
        for (ii = 0; ii < n; ii++)  
        {  
            switch (intersections[ii]->intersect_type)  
            {  
                case UF_MODL_INTERSECT_POINT:  
                    UF_CALL(UF_KF_make_point(intersections[ii]->  
                        intersect.point.coords, &value));  
                    UF_CALL(UF_KF_list_push(results, value, &results));  
                    break;  
                case UF_MODL_INTERSECT_COINCIDE:  
                    UF_CALL(UF_KF_ask_fusion_object(intersections[ii]->  
                        intersect.coincide.identifier, &instance));  
                    UF_CALL(UF_KF_make_instance(instance, &value));  
                    UF_CALL(UF_KF_list_push(results, value, &results));  
                    break;  
                case UF_MODL_INTERSECT_CURVE:  
                    UF_CALL(UF_KF_ask_fusion_object(intersections[ii]->  
                        intersect.curve.identifier, &instance));  
                    UF_CALL(UF_KF_make_instance(instance, &value));  
                    UF_CALL(UF_KF_list_push(results, value, &results));  
                    break;  
                default:  
                    break;  
            }  
            UF_free(intersections[ii]);  
        }  
        if (n > 0) UF_free(intersections);  
        UF_CALL(UF_KF_make_list(results, rv));  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码用于实现NX二次开发中的知识融合函数，用于获取两个知识融合对象实例之间的交点或实例，并将其返回给知识融合环境。
>
> 具体来说，代码首先定义了两个对象实例的输入，然后通过UF_MODL_intersect_objects函数获取两者之间的交点或实例信息。根据交点或实例的类型，将其转换为知识融合环境中的点实例或曲线实例，最后将这些实例封装成知识融合列表返回给调用方。
>
> 关键点包括：
>
> 1. 使用知识融合函数获取输入的两个对象实例。
> 2. 获取两个对象的类型和子类型，如果是特征，则提取对应的实体。
> 3. 调用UF_MODL_intersect_objects函数获取交点或实例信息。
> 4. 根据交点或实例的类型，将其转换为知识融合环境中的点实例或曲线实例。
> 5. 将转换后的实例封装成知识融合列表返回给调用方。
> 6. 释放相关内存，结束知识融合函数调用。
>
> 该函数实现了NX知识融合环境中对象交点的查询，为后续的自动化设计流程提供了基础。
>
