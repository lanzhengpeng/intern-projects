### 【3162】list all solids transforms 列出所有实体的变换信息

#### 代码

```cpp
    /*HEAD LIST_ALL_SOLIDS_TRANSFORMS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_assem.h>  
    #include <uf_view.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 版本中新增的函数。文档要求只提供翻译，不添加任何其他内容，因此翻译如下：

UF_print_syslog 是 V18 版本中新增的函数。 */  
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
    static void report_double_array(char *name, double *array, int rows, int cols)  
    {  
        int  
            ii,  
            jj,  
            kk = 0;  
        printf("%s\n", name);  
        for (ii = 0; ii < rows; ii++)  
        {  
            printf("\t");  
            for (jj = 0; jj < cols; jj++)  
            {  
                printf("%f", array[kk++]);  
                if (jj < cols - 1) printf(", ");  
            }  
            printf("\n");  
        }  
    }  
    static void report_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner;  
        char  
            name[UF_OBJ_NAME_LEN + 1];  
        if (object != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
            printf("%s = %d, type = %d, subtype = %d, ", what, object, type,  
                subtype);  
            if (UF_OBJ_is_object_a_promotion(object)) printf("promotion, ");  
            if (UF_ASSEM_is_occurrence(object)) printf("occurrence, ");  
            if (!UF_CALL(UF_OBJ_ask_owning_part(object, &owner)))  
                printf("owned by %d, ", owner);  
            if (!uc6409(object, &status, name) && (status == 1))  
                printf("view dependent in %s, ", name);  
            if (!UF_OBJ_ask_name(object, name)) printf("named %s, ", name);  
            status = UF_OBJ_ask_status(object);  
            printf("status %d\n", status);  
        }  
        else printf("%s = %d (NULL_TAG)\n", what, object);  
    }  
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void report_component_data(tag_t comp)  
    {  
        double  
            csys_matrix[9],  
            origin[3],  
            transform[4][4];  
        char  
            instance_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_name[MAX_FSPEC_SIZE+1],  
            refset_name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL(UF_ASSEM_ask_component_data(comp, part_name, refset_name,  
            instance_name, origin, csys_matrix, transform));  
        printf("\npart_name = %s\n", part_name);  
        printf("refset_name = %s\n", refset_name);  
        printf("instance_name = %s\n", instance_name);  
        report_double_array("origin", origin, 1, 3);  
        report_double_array("csys_matrix", csys_matrix, 3, 3);  
        report_double_array("transform", (double *)transform, 4, 4);  
    }  
    static void do_it(void)  
    {  
        int  
            ii = 0;  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            part_occ;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        double  
            trans[4][4];  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            sprintf(name, "%d.  ", ++ii);  
            report_object_info(name, body);  
            if (UF_ASSEM_is_occurrence(body))  
            {  
                UF_CALL(UF_ASSEM_ask_transform_of_occ(body, trans));  
                report_double_array("trans", (double *)trans, 4, 4);  
                part_occ = UF_ASSEM_ask_part_occurrence(body);  
                report_component_data(part_occ);  
            }  
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

> 这段代码是用于NX二次开发的C语言程序，其主要功能是遍历当前显示部件中的所有实体，并打印出每个实体的信息，包括类型、子类型、名称、状态、所属部件等。此外，如果实体是实例，还会打印出其变换矩阵和组件数据。程序的关键步骤包括：
>
> 1. 初始化UF函数库
> 2. 遍历当前显示部件中的所有实体
> 3. 对于每个实体，查询其类型、子类型、名称、状态等信息，并打印出来
> 4. 如果实体是实例，查询其变换矩阵和组件数据，并打印出来
> 5. 终止UF函数库
>
> 此外，程序还定义了一个错误报告函数report_error，用于在UF函数调用出错时打印错误信息。整体而言，这段代码通过UF函数库实现了遍历和打印NX部件中实体信息的功能，是一个典型的NX二次开发示例。
>
