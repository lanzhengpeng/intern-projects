### 【0154】ask annotation text origin abs 询问注释文本的绝对原点

#### 代码

```cpp
    static void map_matrix2abs(tag_t input_matrix, double vec[3])  
    {  
        double  
            abs_mx[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            csys[12],  
            mx[12];  
        int  
            irc;  
        UF_CALL(UF_CSYS_ask_matrix_values(input_matrix, &csys[3]));  
        FTN(uf5940)(csys, abs_mx, mx, &irc);  
        UF_CALL(irc);  
        FTN(uf5941)(vec, mx);  
        UF_VEC3_sub(vec, csys, vec);  
    }  
    static logical ask_annotation_text_origin_abs(tag_t obj, double origin[3])  
    {  
        int  
            n_text;  
        tag_t  
            matrix;  
        UF_DRF_draft_aid_text_info_t  
            *text_info;  
        if (UF_CALL(UF_DRF_ask_draft_aid_text_info(obj, &n_text, &text_info)) ||  
            (n_text == 0) ||  
            UF_CALL(UF_CSYS_ask_matrix_of_object(obj, &matrix))) return FALSE;  
        UF_VEC3_copy(text_info[0].origin, origin);  
        map_matrix2abs(matrix, origin);  
        UF_DRF_free_text(n_text, &text_info);  
        return TRUE;  
    }

```

#### 代码解析

> 这段代码是用于NX Open C++ API进行NX二次开发的一段代码，其主要功能是获取NX中注释文本的绝对坐标。
>
> 代码中定义了两个函数：
>
> 1. map_matrix2abs函数：该函数将NX对象相对于其坐标系变换到绝对坐标系。它接收一个输入矩阵（代表对象坐标系），输出一个绝对坐标系下的向量。
> 2. ask_annotation_text_origin_abs函数：该函数获取NX注释文本的绝对坐标。它首先获取文本注释的信息，包括文本数量和文本信息数组。然后获取文本所在对象的坐标系矩阵，并调用map_matrix2abs函数将文本原点坐标转换到绝对坐标系。最后返回转换后的绝对坐标。
>
> 整体来说，这段代码实现了NX中注释文本原点坐标的获取，通过坐标系变换将相对坐标转换为绝对坐标，为后续的文本注释定位等二次开发提供了基础。
>
