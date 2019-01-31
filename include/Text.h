#ifndef TEXT_H
#define TEXT_H


class Text
{
    public:
        Text();
        virtual ~Text();

        bool expired() {
			return textLife > duration;
		}

        void showText( float dt,  int textType );

    protected:
    private:

        const static int duration = 1500;
        char* textOutput;
        int textLife ;
};

#endif // TEXT_H
