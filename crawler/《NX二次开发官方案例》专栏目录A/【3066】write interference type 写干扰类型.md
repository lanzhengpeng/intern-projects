### 【3066】write interference type 写干扰类型

#### 代码

```cpp
    static void write_interference_type(int interf_type)  
    {  
        switch (interf_type)  
        {  
            case UF_CLEAR_SOFT_INTERFERENCE:  
                WRITE("UF_CLEAR_SOFT_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_TOUCH_INTERFERENCE:  
                WRITE("UF_CLEAR_TOUCH_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_HARD_INTERFERENCE:  
                WRITE("UF_CLEAR_HARD_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_CONT_1_IN_2_INTERFERENCE:  
                WRITE("UF_CLEAR_CONT_1_IN_2_INTERFERENCE - ");  
                break;  
            case UF_CLEAR_CONT_2_IN_1_INTERFERENCE:  
                WRITE("UF_CLEAR_CONT_2_IN_1_INTERFERENCE - ");  
                break;  
            default:  
                WRITE("Unknown Interference Type - ");  
        }  
        WRITE_D(interf_type);  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的，用于打印不同类型的界面干扰类型。
>
> 主要功能包括：
>
> 1. 使用switch语句根据不同的界面干扰类型(interf_type)打印对应的描述。
> 2. 每种干扰类型都有对应的case，打印描述并加上一个横线。
> 3. 如果是未知类型的干扰，则打印"Unknown Interference Type - "。
> 4. 使用WRITE和WRITE_D宏来打印描述和干扰类型值。
> 5. 这段代码的主要作用是调试，将干扰类型转换为可读的字符串描述，以便在调试时输出。
>
> 总体来说，这段代码实现了根据干扰类型打印不同描述的功能，可用于调试或输出干扰类型信息。
>
