from django.shortcuts import render

def index(request):
    context = {
        'tail_apps' : ('xapian', 'moses', 'masstree', 'img-dnn', 'shore', 'silo', 'sphinx'),
        'other_apps': ('redis',),
    }
    return render(request, 'index.html', context)

