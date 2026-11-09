import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("/Users/fintan/Documents/GitHub/ski-tracker/datalog.csv")
print(df)

t_us = (df["t_us"])
ax_g = (df["ax_g"])
ay_g = (df["ay_g"])
az_g = (df["az_g"])
gx_dps = (df["gx_dps"])
gy_dps = (df["gy_dps"])
gz_dps = (df["gz_dps"])

plt.plot(t_us, ax_g)
plt.plot(t_us, ay_g)
plt.plot(t_us, az_g)

plt.show()