import pandas as pd 
import glob 
import os
import plotly.express as px
import numpy as np 
import matplotlib.pyplot as plt
from sklearn.metrics import r2_score

import pickle

from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression


training_dataset=pd.read_csv("./trainingData_csv.csv",engine='python',sep=',')

#print(training_dataset.columns)

features_X = training_dataset[['sched_enter_ini', 'mem_alloc_ini_1MB', 'mem_alloc_ini_100MB',
       'mem_alloc_ini_500MB', 'mem_alloc_ini_500', 'mem_read_write_1kB_ini',
       'mem_read_write_10K_ini', 'mem_read_write_1MB_ini',
       'mem_read_write_500MB_ini', 'mem_read_write_100MB_ini',
       'disk_tx_speed_100MB_ini', 'disk_tx_speed_10MB_ini',
       'disk_rx_speed_100_in', 'disk_rx_speed_10_in', 'net_tx_speed_in',
       'net_rx_speed_ini', 'cpu', 'mem', 'disk_tx', 'disk_rx', 'net_tx',
       'net_rx','sched_enter_ini_n', 'mem_alloc_ini_1MB_n',
       'mem_alloc_ini_100MB_n', 'mem_alloc_ini_500MB_n', 'mem_alloc_ini_500_n',
       'mem_read_write_1kB_ini_n', 'mem_read_write_10K_ini_n',
       'mem_read_write_1MB_ini_n', 'mem_read_write_500MB_ini_n',
       'mem_read_write_100MB_ini_n', 'disk_tx_speed_100MB_ini_n',
       'disk_tx_speed_10MB_ini_n', 'disk_rx_speed_100_in_n',
       'disk_rx_speed_10_in_n', 'net_tx_speed_in_n']]



Y_real = training_dataset[['cpu_n', 'mem_n', 'disk_tx_n', 'disk_rx_n', 'net_tx_n', 'net_rx_n']]


# Add some noise

X_train, X_test, y_train, y_test = train_test_split(features_X, Y_real, test_size=0.5)


reg_nnls = LinearRegression(positive=True)
y_pred_nnls = reg_nnls.fit(X_train, y_train).predict(X_test)
r2_score_nnls = r2_score(y_test, y_pred_nnls)
print("NNLS R2 score", r2_score_nnls)

filename = 'finalized_model.sav'
pickle.dump(reg_nnls, open(filename, 'wb'))
