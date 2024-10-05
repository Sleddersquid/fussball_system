import lgpio
import time

# Initialize the GPIO chip and set pin 23 as output
chip = lgpio.gpiochip_open(0)  # Open GPIO chip 0
lgpio.gpio_claim_output(chip, 23)  # Claim GPIO pin 23

lgpio.gpio_write(chip, 23, 1)
print("Pin 23 is HIGH")
time.sleep(2)


# Set pin 23 low (turn off)
lgpio.gpio_write(chip, 23, 0)
print("Pin 23 is LOW")

# Close GPIO chip
lgpio.gpiochip_close(chip)
