import json
import numpy as np
import matplotlib.pyplot as pl


# create figure
pl.figure(1)
pl.axis('equal')
pl.xlabel('x [m]')
pl.ylabel('y [m]')

with open('../map.json') as json_file:
    data = json.load(json_file)
    for p in data['data']:

        # create vectors
        x = np.array([])
        y = np.array([])

        # fill vectors
        for d in p['points']:
            x = np.append(x, d['x'])
            y = np.append(y, d['y'])

        # plot vectors
        pl.plot(x, y, ':k')


pl.show(block=True)
