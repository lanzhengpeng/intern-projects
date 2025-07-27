### 【0183】ask dims of sketch exps 询问草图表达式的尺寸

#### 代码

```cpp
    static void ask_dims_of_sketch_exps(tag_t sketch, int n_exps, tag_t *dim_exps)  
    {  
        int  
            ii, jj,  
            n_dims,  
            status;  
        tag_t  
            *dims,  
            exp;  
        double  
            value;  
        char  
            exp_string[256];  
        UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
        for (ii = 0; ii < n_dims; ii++)  
        {  
            UF_CALL(UF_SKET_ask_dim_status(dims[ii], &exp, exp_string, &value,  
                &status));  
            for (jj = 0; jj < n_exps; jj++)  
                if (exp == dim_exps[jj])  
                {  
                    dim_exps[jj] = dims[ii];  
                    break;  
                }  
        }  
        if (n_dims > 0) UF_free(dims);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，其主要功能是获取NX草图中的指定表达式的维度。
>
> 代码流程如下：
>
> 1. 首先调用UF_SKET_ask_dimensions_of_sketch函数，获取草图中的所有维度及其tag。
> 2. 遍历获取到的所有维度，并针对每个维度，调用UF_SKET_ask_dim_status函数，以获取其表达式tag、表达式字符串、表达式值和状态。
> 3. 在遍历过程中，将每个维度的表达式tag与输入的dim_exps数组中的表达式tag进行对比。如果匹配，则将对应的dim_exps数组的元素更新为当前维度的tag。
> 4. 最后释放维度数组所占用的内存。
>
> 通过这段代码，可以方便地获取NX草图中指定表达式对应的维度tag，进而进行后续的查询或修改操作。
>
