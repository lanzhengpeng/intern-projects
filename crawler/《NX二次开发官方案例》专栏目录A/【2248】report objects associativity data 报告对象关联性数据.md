### 【2248】report objects associativity data 报告对象关联性数据

#### 代码

```cpp
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_F(X) (write_double_to_listing_window(#X, X))  
    static void write_double_to_listing_window(char *title, double n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %f\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
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
            blanks[UF_UI_MAX_STRING_LEN+1],  
            msg[UF_UI_MAX_STRING_LEN+1];  
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
    #define RX(X) report_simple_object_info(#X, X)  
    static void ask_object_type_descriptor(int type, int subtype, char *descriptor)  
    {  
        int  
            ii,  
            a_type;  
        char  
            a_line[MAX_LINE_SIZE+1],  
            api_dir[UF_CFI_MAX_PATH_NAME_SIZE],  
            *base_dir,  
            define[133],  
            *ptr,  
            uoth_file[UF_CFI_MAX_PATH_NAME_SIZE];  
        FILE  
            *in_file;  
        switch (type)  
        {  
            case UF_circle_type: /* non-std format in the header file 译:头文件中的非标准格式翻译如下：

头文件中的非标准格式 */  
                strcpy(descriptor, "arc");  
                return;  
            case UF_OBJ_EXPR_TYPE:  
                strcpy(descriptor, "expression");  
                return;  
            case UF_OBJ_PART_TYPE:  
                strcpy(descriptor, "part");  
                return;  
            default:  
                break;  
        }  
        UF_CALL(UF_translate_variable("UGII_BASE_DIR", &base_dir));  
        UF_CALL(uc4575(base_dir, 100, "ugopen", api_dir));  
        UF_CALL(uc4575(api_dir, 0, "uf_object_types.h", uoth_file));  
        if ((in_file = fopen(uoth_file, "r")) == NULL)  
        {  
            strcpy(descriptor, "(uf_object_types.h not found)");  
            return;  
        }  
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:如果找不到的话 */  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if ((ptr = strstr(define, "_type")) && (a_type == type))  
                {  
                    *ptr = '\0';  
                    strcpy(descriptor, define);  
                    if (strcmp(define, "gfem_load")) break;  
                }  
            }  
        }  
        while (!feof(in_file))  
        {  
            fgets(a_line, MAX_LINE_SIZE, in_file);  
            if ((ii = sscanf(a_line, "#define UF_%s %d", define, &a_type)) == 2)  
            {  
                if (ptr = strstr(define, "_type")) break;  
                if (a_type == subtype)  
                {  
                    if (ptr = strstr(define, "_subtype")) *ptr = '\0';  
                    if (strstr(define, descriptor) == define)  
                        strcpy(descriptor, define);  
                    else  
                    {  
                        strcat(descriptor, "/");  
                        strcat(descriptor, define);  
                    }  
                    break;  
                }  
            }  
        }  
        fclose(in_file);  
        while (ptr = strchr(descriptor, '_')) *ptr = ' ';  
    }  
    static void report_simple_object_info(char *what, tag_t object)  
    {  
        int  
            status,  
            subtype,  
            type;  
        tag_t  
            owner,  
            part = UF_PART_ask_display_part();  
        char  
            name[UF_OBJ_NAME_LEN + 1],  
            msg[133];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d", what, object);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (object == NULL_TAG)  
        {  
            UF_CALL(UF_UI_write_listing_window(" (NULL_TAG)\n"));  
            return;  
        }  
        status = UF_OBJ_ask_status(object);  
        sprintf(msg, ", status: %d", status);  
        UF_CALL(UF_UI_write_listing_window(msg));  
        if (status == UF_OBJ_DELETED)  
        {  
            UF_UI_write_listing_window("\n");  
            return;  
        }  
        if (!UF_OBJ_ask_name(object, name))  
        {  
            UF_UI_write_listing_window(", named: ");  
            UF_UI_write_listing_window(name);  
        }  
        UF_CALL(UF_OBJ_ask_type_and_subtype(object, &type, &subtype));  
        ask_object_type_descriptor(type, subtype, msg);  
        UF_UI_write_listing_window(", type: ");  
        UF_UI_write_listing_window(msg);  
        if (UF_OBJ_is_object_a_promotion(object))  
            UF_UI_write_listing_window(", promotion");  
        if (UF_ASSEM_is_occurrence(object))  
        {  
            UF_CALL(UF_UI_write_listing_window(", occurrence"));  
            owner = UF_ASSEM_ask_part_occurrence(object);  
            if (owner != UF_ASSEM_ask_root_part_occ(part))  
            {  
                UF_CALL(UF_OBJ_ask_name(owner, msg));  
                UF_CALL(UF_UI_write_listing_window(" (from "));  
                UF_CALL(UF_UI_write_listing_window(msg));  
                UF_CALL(UF_UI_write_listing_window(")"));  
            }  
        }  
        UF_UI_write_listing_window("\n");  
    }  
    static void report_objects_associativity_data(tag_t dim)  
    {  
        int  
            ii,  
            n_assoc;  
        UF_DRF_object_assoc_data_t  
            *assoc_data;  
        UF_CALL(UF_DRF_ask_associativity_data(dim, &n_assoc, &assoc_data));  
        WRITE_D(n_assoc);  
        for (ii = 0; ii < n_assoc; ii++)  
        {  
            WRITE_D(ii);  
            RX(assoc_data[ii].assoc_object_1);  
            RX(assoc_data[ii].assoc_object_2);  
            RX(assoc_data[ii].object_view);  
            WRITE_D(assoc_data[ii].point_type);  
            WRITE_D(assoc_data[ii].line_type);  
            WRITE3F(assoc_data[ii].base_pt_1);  
            WRITE3F(assoc_data[ii].base_pt_2);  
            WRITE_F(assoc_data[ii].entered_angle);  
            WRITE3F(assoc_data[ii].assoc_point);  
        }  
        if (n_assoc > 0) UF_free(assoc_data);  
    }

```

#### 代码解析

> 这段NX二次开发代码主要用于报告和输出NX模型中各种对象的信息，包括：
>
> 1. 宏定义：定义了一些宏，用于输出整数、浮点数和浮点数数组到列表窗口。
> 2. 报告简单对象信息：用于输出一个NX对象的基本信息，包括名称、状态、类型等。
> 3. 询问对象类型描述符：根据对象类型和子类型，查找对应的描述符。
> 4. 报告对象关联性数据：输出一个维度对象的关联性数据，包括关联对象、视图、点类型、线类型等。
> 5. NX对象信息输出：通过调用NX提供的API函数，获取对象的基本信息和关联性数据，并格式化输出到列表窗口。
> 6. 辅助函数：包括打开列表窗口、写入列表窗口等辅助函数。
>
> 总体来说，这段代码通过NX提供的API，实现了一个简单的NX对象信息报告工具，可以帮助用户查看和了解NX模型中各个对象的基本情况。
>
