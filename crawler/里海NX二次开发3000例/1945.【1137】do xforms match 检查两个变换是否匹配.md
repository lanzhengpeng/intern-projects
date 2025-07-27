### 【1137】do xforms match 检查两个变换是否匹配

#### 代码

```cpp
    static logical do_xforms_match(tag_t one, tag_t two)  
    {  
        int  
            match;  
        double  
            m1[16],  
            m2[16],  
            p1[3],  
            p2[3];  
        UF_CALL(UF_SO_ask_point_of_xform(one, p1));  
        UF_CALL(UF_SO_ask_point_of_xform(two, p2));  
        UF_VEC3_is_equal(p1, p2, 0, &match);  
        if (!match) return FALSE;  
        UF_CALL(UF_SO_ask_matrix_of_xform(one, m1));  
        UF_CALL(UF_SO_ask_matrix_of_xform(two, m2));  
        UF_VEC4_is_equal(m1, m2, 0, &match);  
        if (!match) return FALSE;  
        UF_VEC4_is_equal(&m1[3], &m2[3], 0, &match);  
        if (!match) return FALSE;  
        UF_VEC4_is_equal(&m1[7], &m2[7], 0, &match);  
        if (!match) return FALSE;  
        UF_VEC4_is_equal(&m1[10], &m2[10], 0, &match);  
        if (!match) return FALSE;  
        return TRUE;  
    }

```

#### 代码解析

> 这段NX Open C++代码的功能是判断两个NX变换对象是否匹配。其主要步骤包括：
>
> 1. 获取两个变换对象的参考点坐标，并比较它们是否相等。
> 2. 获取两个变换对象的4x4矩阵，并逐列比较它们是否相等。
> 3. 如果所有坐标和矩阵元素都相等，则返回true，表示两个变换对象匹配。
> 4. 否则返回false，表示不匹配。
>
> 该函数使用了NX Open API中的UF_SO_ask_point_of_xform、UF_SO_ask_matrix_of_xform、UF_VEC3_is_equal和UF_VEC4_is_equal等函数来实现上述功能。
>
> 通过比较变换对象的参考点和变换矩阵，该函数可以准确判断两个变换对象是否相同，从而实现变换对象的匹配检查。
>
