### 【2244】report object preferences 获取对象参数设置

#### 代码

```cpp
    static void report_object_preferences(tag_t object)  
    {  
        int  
            ii,  
            mpi[100];  
        double  
            mpr[70];  
        char  
            rad[27],  
            dia[27],  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_DRF_ask_object_preferences(object, mpi, mpr, rad, dia));  
        for (ii = 0; ii < 100; ii++)  
        {  
            sprintf(msg, "mpi[%d] = %d\n", ii, mpi[ii]);  
            ECHO(msg);  
        }  
        for (ii = 0; ii < 70; ii++)  
        {  
            sprintf(msg, "mpr[%d] = %f\n", ii, mpr[ii]);  
            ECHO(msg);  
        }  
        WRITE_S(rad);  
        WRITE_S(dia);  
    }

```

#### 代码解析

> 这段代码是NX Open的C++ API编写的二次开发代码，用于查询和打印NX对象（如特征、实体等）的偏好设置。
>
> 具体来说，代码首先定义了几个数组，用于存储从NX对象中获取的偏好设置信息，包括整数数组mpi、双精度数组mpr、字符数组rad和dia。
>
> 接着，调用UF_DRF_ask_object_preferences函数，从指定的NX对象中获取其偏好设置信息，并存储到上述数组中。
>
> 最后，通过两个for循环遍历mpi和mpr数组，使用sprintf函数格式化输出每个元素的值。同时，调用WRITE_S函数打印出rad和dia数组的值。
>
> 总体来说，这段代码的功能是查询NX对象的偏好设置信息，并以格式化的形式打印出来，以便进行调试或查看。
>
