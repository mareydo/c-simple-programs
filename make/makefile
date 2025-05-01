CC gcc
CFLAGS -Wall

TARGET myCode

SRCS main.c lib.c
LIBS lib.h

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c %.h
    $(CC) $(CFLAGS) -c $< -o $@


clean:
    rm -f $(OBJS) $(TARGET)