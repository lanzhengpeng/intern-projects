### 【2440】report whether selected faces are equivalent and or identical 报告选定的面是否等价和或相同

#### 代码

```cpp
    /*  
    report_whether_selected_faces_are_equivalent_and_or_identical.c  
    UF_MODL_compare() may report that two faces are equivalent,   
    even if they do not share the same location in space.  
    UF_MODL_compare_topology() will not report two faces to be   
    identical if they occupy different locations, greater than  
    the tolerance supplied.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    /* qq3123197280 */  
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
    /* qq3123197280 */  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static void do_it(void)  
    {  
        int  
            relation = 1,       /*    1 = Geometric Equivalence     里海译:1 = 几何等价 */  
            equivalent = 0,  
            status = -1;  
        tag_t  
            face_1 = NULL_TAG,  
            face_2 = NULL_TAG;  
        char  
            msg[132] = { "" };  
        while ( ( face_1 = select_a_face( "Select First Face" )) != NULL_TAG &&  
                ( face_2 = select_a_face( "Select Second Face" )) != NULL_TAG  )  
        {  
            UF_CALL( UF_MODL_compare( face_1, face_2, relation, &equivalent ));  
            if( equivalent )  
            {  
                strcpy_s( msg, sizeof(msg), "Compare: Selected Faces Are Equivalent\n\n" );  
            }  
            else  
            {  
                strcpy_s( msg, sizeof(msg), "Compare: Selected Faces Are NOT Equivalent\n\n" );  
            }  
            ECHO(msg);   
            UF_CALL(UF_MODL_compare_topology(face_1, face_2, 0.01,0.01, &status));  
            ECHO("Status from compare_topology is %d, meaning: ", status);  
            if(status == COMPARE_NOMATCH)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_NOMATCH\n");  
            }  
            if(status == COMPARE_FACES_IDENTICAL)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_FACES_IDENTICAL\n");  
            }  
            if(status == COMPARE_FACES_EQUIVALENT)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_FACES_EQUIVALENT\n");  
            }  
            if(status == COMPARE_SURFACES_IDENTICAL)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_SURFACES_IDENTICAL\n");  
            }  
            if(status == COMPARE_SURFACES_EQUIVALENT)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_SURFACES_EQUIVALENT\n");  
            }  
            if(status == COMPARE_EDGES_IDENTICAL)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_EDGES_IDENTICAL\n");  
            }  
            if(status == COMPARE_CURVES_IDENTICAL)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_CURVES_IDENTICAL\n");  
            }  
            if(status == COMPARE_CURVES_EQUIVALENT)  
            {  
                sprintf_s(msg, sizeof(msg), "%s\n", "COMPARE_CURVES_EQUIVALENT\n");  
            }  
            ECHO(msg);  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个echo函数，用于打印信息到NX的输出窗口和日志文件。
> 2. 定义了一个report_error函数，用于在函数调用失败时打印错误信息。
> 3. 定义了一个mask_for_faces函数，用于设置选择过滤器，只允许选择实体和特征的面。
> 4. 定义了一个select_a_face函数，用于让用户选择一个面，并返回面tag。
> 5. 在do_it函数中，使用select_a_face函数让用户选择两个面，然后调用UF_MODL_compare函数判断这两个面是否几何等价，并打印结果。
> 6. 接着调用UF_MODL_compare_topology函数比较两个面的拓扑结构，并打印比较结果状态。
> 7. 在ufusr函数中初始化NX环境，调用do_it函数，然后终止NX环境。
> 8. 定义了ufusr_ask_unload函数，用于卸载NX加载项。
>
> 总的来说，这段代码实现了选择两个面，并比较它们是否几何等价以及拓扑结构是否相同的功能。通过打印信息到NX的输出窗口和日志文件，可以让用户直观地看到比较结果。
>
