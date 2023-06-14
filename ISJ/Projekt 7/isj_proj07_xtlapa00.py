#!/usr/bin/env python3
from functools import wraps
import math

class TooManyCallsError(Exception):
    def __init__(self, message):
        # Call the base class constructor with the parameters it needs
        super().__init__('function "pyth" - ' + message)

def limit_calls(max_calls = 2, error_message_tail = 'called too often'):
    def decorator(func):
        def wrapper(*args, **kwargs):
            calls = getattr(wrapper, 'calls', 0)
            calls += 1
            if calls > max_calls:
              raise TooManyCallsError(error_message_tail)

            setattr(wrapper, 'calls', calls)

            return func(*args, **kwargs)

        setattr(wrapper, 'calls', 0)
        return wrapper

    return decorator

def ordered_merge(*args, selector =[]):
       if not selector:
           return selector
       lenght = len(selector)
       seznam = []
       for x in range(lenght):
           seznam.append(selector[x])
           index = selector[x]
           yield args[index][seznam.count(selector[x])-1]

'''print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11), selector=[2, 3, 0, 1, 3, 1])))'''
'''print(list(ordered_merge('abcde', [1, 2, 3], (3.0, 3.14, 3.141), range(11, 44, 11))))'''

class  Log():
    def __init__(self, file):
        self.fl = open(file, "w")
    def logging(self, msg):
        self.fl.write(msg + "\n")
    def __enter__(self):
        self.fl.write("Begin\n")
        return self
    def __exit__(self, exc_type, exc_val, exc_tb):
        self.fl.write("End\n")
        self.fl.close()