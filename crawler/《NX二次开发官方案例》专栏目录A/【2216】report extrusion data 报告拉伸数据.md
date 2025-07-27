### 【2216】report extrusion data 报告拉伸数据

#### 代码

```cpp
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_L(X) (write_logical_to_listing_window(#X, X))  
    static void write_logical_to_listing_window(char *title, logical flag)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        if (flag)  
            sprintf(msg, "%s = TRUE\n", title);  
        else  
            sprintf(msg, "%s = FALSE\n", title);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = \"%s\"\n", title, string);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
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
            case UF_circle_type: /* non-std format in the header file 译:非标准格式在头文件中的翻译为：非标准格式 */  
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
        strcpy(descriptor, "Unknown type");  /* In case its not found 译:In case its not found, 如果没有找到 */  
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
    #define WRITEnTt(n,X) (report_object_array_types(n, #X, X))  
    static void report_object_array_types(int n_objects, char *title, tag_t *objects)  
    {  
        int  
            ii,  
            subtype,  
            type;  
        char  
            msg[UF_UI_MAX_STRING_LEN+1],  
            type_desc[MAX_LINE_SIZE+1];  
        if (n_objects < 1) return;  
        UF_CALL(UF_UI_open_listing_window());  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(objects[ii], &type, &subtype));  
            ask_object_type_descriptor(type, subtype, type_desc);  
            sprintf(msg, "%s[%d] = %d, %s\n", title, ii, objects[ii], type_desc);  
            UF_CALL(UF_UI_write_listing_window(msg));  
        }  
    }  
    static void report_extrusion_data(tag_t feature)  
    {  
        logical  
            region_specified,  
            solid_creation;  
        int  
            n_objects;  
        tag_t  
            *objects;  
        double  
            direction[3],  
            region_point[3];  
        char  
            *limits[2],  
            *offsets[2],  
            *taper_angle;  
        UF_MODL_SWEEP_TRIM_object_p_t  
            trim_ptr;  
        if (UF_CALL(UF_MODL_ask_extrusion(feature, &n_objects, &objects,  
            &trim_ptr, &taper_angle, limits, offsets, region_point,  
            &region_specified, &solid_creation, direction))) return;  
        WRITE_D(n_objects);  
        WRITEnTt(n_objects, objects);  
        if (n_objects > 0) UF_free(objects);  
        if (trim_ptr != NULL)  
        {  
            WRITE_D(trim_ptr->trim_count);  
            WRITEnTt(trim_ptr->trim_count,trim_ptr->trim_objects);  
            if (trim_ptr->trim_count > 0) UF_free(trim_ptr->trim_objects);  
            WRITE_D(trim_ptr->sign);  
            WRITE_D(trim_ptr->num_thru_bodies);  
            WRITEnTt(trim_ptr->num_thru_bodies,trim_ptr->thru_bodies);  
            if (trim_ptr->num_thru_bodies > 0) UF_free(trim_ptr->thru_bodies);  
        }  
        WRITE_S(taper_angle);  
        UF_free(taper_angle);  
        WRITE_S(limits[0]);  
        UF_free(limits[0]);  
        WRITE_S(limits[1]);  
        UF_free(limits[1]);  
        WRITE_S(offsets[0]);  
        UF_free(offsets[0]);  
        WRITE_S(offsets[1]);  
        UF_free(offsets[1]);  
        WRITE_L(region_specified);  
        if (region_specified) WRITE3F(region_point);  
        WRITE_L(solid_creation);  
        WRITE3F(direction);  
    }

```

#### 代码解析

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了几个宏，用于将不同类型的数据写入到NX的Listing窗口中，例如WRITE3F用于写入浮点数数组，WRITE_D用于写入整数等。
> 2. 实现了write_double_array_to_listing_window函数，用于将浮点数数组写入Listing窗口。该函数接受数组名、数组指针、行数和列数，并按指定格式写入数据。
> 3. 实现了write_integer_to_listing_window函数，用于写入整数数据。
> 4. 实现了write_logical_to_listing_window函数，用于写入逻辑值数据。
> 5. 实现了write_string_to_listing_window函数，用于写入字符串数据。
> 6. 实现了ask_object_type_descriptor函数，用于根据NX对象的类型和子类型，从uf_object_types.h头文件中查询并返回对象类型描述符。
> 7. 定义了WRITEnTt宏，用于报告NX对象数组的类型。
> 8. 实现了report_object_array_types函数，用于报告NX对象数组中的对象类型。
> 9. 实现了report_extrusion_data函数，用于报告NX拉伸特征的数据，包括拉伸对象、修剪对象、拉伸方向、区域点等信息，并使用之前定义的宏将数据写入Listing窗口。
>
> 综上所述，这段代码实现了将不同类型的数据写入NX Listing窗口的功能，并提供了查询NX对象类型描述符的功能，最后通过report_extrusion_data函数报告了一个NX拉伸特征的数据。
>
