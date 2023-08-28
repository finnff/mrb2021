import numpy as np


def savitzky_golay_coefficients(window_size, order):
    half_window = (window_size - 1) // 2
    A = np.array(
        [
            [k**i for i in range(order + 1)]
            for k in range(-half_window, half_window + 1)
        ],
        dtype="float",
    )
    b = np.zeros(window_size)
    b[half_window] = 1  # We want to fit to a delta function centered in the window

    # Calculate the least squares solution
    coeffs = np.linalg.lstsq(A, b, rcond=None)[0]

    # Calculate the convolution kernel
    kernel = np.dot(A, coeffs)

    # Scaling the coefficients to integers
    scale_factor = 10**3
    int_kernel = np.round(kernel * scale_factor).astype(int)

    # Calculate the GCD and divide by it
    gcd = np.gcd.reduce(int_kernel)
    int_kernel = int_kernel // gcd

    # Calculate the coefficient divider (the sum of the coefficients)
    coefficientDivider = np.sum(np.abs(int_kernel))

    return int_kernel, coefficientDivider


if __name__ == "__main__":
    for window_size in [5, 7, 9, 11, 13]:
        print(
            "___________________________________________________________________________"
        )
        coeffs, divider = savitzky_golay_coefficients(window_size, 3)
        print(
            f"For window size {window_size}, coefficients are {coeffs.tolist()} with a coefficientDivider of {divider}"
        )

        if any(coeffs < 0):
            print("At least one coefficient is negative, requiring a signed data type.")

        highest_coeff = max(np.abs(coeffs))
        scaled_highest_coeff = highest_coeff * 1023

        print(f"The highest coefficient multiplied by 1023 is {scaled_highest_coeff}")

        if scaled_highest_coeff <= 32767 and scaled_highest_coeff >= -32768:
            print("This would fit in a signed int data type on Arduino.")
        elif scaled_highest_coeff <= 65535:
            print("This would fit in a uint16_t data type on Arduino.")
        elif scaled_highest_coeff <= 4294967295:
            print("This would fit in a uint32_t data type on Arduino.")
        elif scaled_highest_coeff >= -2147483648 and scaled_highest_coeff <= 2147483647:
            print("This would fit in a long int data type on Arduino. 32 bit only!")
        else:
            print("This would require a larger data type than long int on Arduino.")
