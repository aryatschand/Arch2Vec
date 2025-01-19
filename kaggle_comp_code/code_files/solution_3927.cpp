
    
    #include <stdio.h>
    #include <time.h>
    #include <iostream.h>
    using namespace std;
    
    //=============================================================
    
    class Vector {
    
            public:
              Vector( int size = STEP );
    
              virtual int Add( int v );  // zwraca v, gdy pomyslnie, inaczej -1
              int Remove( int v ); // zwraca j.w.
              void RemoveAll(); // wektor bedzie zawieral 0 elementow
              int GetAt( int index ) const;
              virtual int RemoveAt( int index );
              virtual void SetAt( int index, int v );
              int Contains( int v ) const;
              int GetIndex( int val ) const;
    
              int GetSize() const { return num_of_elem; }
              int GetMaxElement() const;
              int GetMinElement() const;
    
              ~Vector();
    
            protected:
              int size;
              int num_of_elem;
              int *elements;
    
              static const int STEP = 100;
    
              void Resize();
    };
    
    
    Vector::Vector( int size )
    {
      num_of_elem = 0;
      this->size = size;
      if ( size > 0 )
        elements = new int[size];
      else
        elements = 0;
    }
    
    
    int Vector::Add( int v )
    {
      Resize();
      elements[num_of_elem++] = v;
      return v;
    }
    
    
    int Vector::GetIndex( int val ) const
    {
      int i;
    
      for ( i=0; i < num_of_elem; i++ )
        if ( elements[i] == val )
          break;
      return ( i < num_of_elem ? i : -1 );
    }
    
    
    int Vector::Remove( int v )
    {
      int i;
    
      for ( i=0; i < num_of_elem && elements[i] != v; i++ );
      if ( i < num_of_elem )
        return RemoveAt( i );
      else
        return -1;
    }
    
    
    void Vector::RemoveAll()
    {
      for ( int i=0; i < num_of_elem; i++ )
        elements[i] = 0;
      num_of_elem = 0;
      Resize();
    }
    
    
    int Vector::GetAt( int index ) const
    {
      if ( index < num_of_elem )
        return elements[index];
      else
        return -1;
    }
    
    
    int Vector::RemoveAt( int index )
    {
      int ret = GetAt( index );
      Resize();
      for ( int i=index; i < num_of_elem-1; i++ )
        elements[i] = elements[i+1];
      num_of_elem--;
      return ret;
    }
    
    
    void Vector::SetAt( int index, int v )
    {
      while ( index >= num_of_elem )
        Add( -1 );
      elements[index] = v;
    }
    
    
    int Vector::Contains( int v ) const
    {
      for ( int i=0; i < num_of_elem; i++ )
        if ( elements[i] == v )
          return 1;
      return 0;
    }
    
    
    int Vector::GetMaxElement() const
    {
      if ( num_of_elem == 0 )
        return -1;
    
      int max = elements[0];
      for ( int i=1; i < num_of_elem; i++ )
        if ( elements[i] > max )
          max = elements[i];
      return max;
    }
    
    
    int Vector::GetMinElement() const
    {
      if ( num_of_elem == 0 )
        return -1;
    
      int min = elements[0];
      for ( int i=1; i < num_of_elem; i++ )
        if ( elements[i] < min )
          min = elements[i];
      return min;
    }
    
    
    Vector::~Vector()
    {
      delete [] elements;
    }
    
    
    void Vector::Resize()
    {
      int *n, i;
    
      if ( num_of_elem >= size || size - num_of_elem >= 2*STEP ) {
        size = num_of_elem + STEP;
        n = new int[size];
        for ( i=0; i < num_of_elem; i++ )
          n[i] = elements[i];
        if ( elements != 0 )
          delete [] elements;
        elements = n;
      }
    }
    
    
    //============================================================
    
    class Array {
            public:
              Array( int size = 100 );
    
              int GetAt( int x, int y ) const;
              void SetAt( int x, int y, int v );
    
              int GetSize() const { return num_of_elem; }
    
              void NewRowAndColumn();
              void RemoveRowAndColumn( int i );
              void Clear(); // tablica bedzie pusta
    
              ~Array();
    
            private:
              int size;
              int num_of_elem;
              int *m;
    
              static const int STEP = 5;
    
              void Resize( int new_size );
    };
    
    
    Array::Array( int size )
    {
      if ( size > 0 ) {
        m = new int[size*size];
      } else
        m = 0;
      this->size = size;
      num_of_elem = 0;
    }
    
    
    int Array::GetAt( int x, int y ) const
    {
      if ( x < num_of_elem && y < num_of_elem )
        return m[y*size+x];
      else
        return -1;
    }
    
    
    void Array::SetAt( int x, int y, int v )
    {
      int max = ( x>y ? x:y ), i, j;
    
      Resize( max+1 );
      if ( max >= num_of_elem ) {
        for ( i=num_of_elem; i <=max; i++ )
          for ( j=0; j <= max; j++ )
            m[i*size+j] = m[j*size+i] = 0;
        num_of_elem = max+1;
      }
      m[y*size+x] = v;
    }
    
    
    void Array::RemoveRowAndColumn( int i )
    {
      int k, j;
    
      for ( k=i; k < num_of_elem; k++ )
        for ( j=0; j < num_of_elem; j++ )
          SetAt( k, j, GetAt( k+1, j ) );
    
      for ( k=i; k < num_of_elem; k++ )
        for ( j=0; j < num_of_elem; j++ )
          SetAt( j, k, GetAt( j, k+1 ) );
    
      num_of_elem--;
    }
    
    
    void Array::NewRowAndColumn()
    {
      SetAt( num_of_elem, num_of_elem, 0 );
    }
    
    
    void Array::Clear()
    {
      for ( int i=0; i < size*size; i++ )
        m[i] = 0;
      num_of_elem = 0;
    }
    
    
    Array::~Array()
    {
      if ( m != 0 )
        delete [] m;
    }
    
    
    void Array::Resize( int new_size )
    {
      int *n, i, j, s;
    
      if ( new_size >= size-2 ) {
        s = new_size + 1 + STEP;
        n = new int[s*s];
        //if ( !n )
        //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
        for ( i=0; i < s*s; i++ )
          n[i] = 0;
        for ( i=0; i < num_of_elem; i++ )
          for ( j=0; j < num_of_elem; j++ )
            n[i*s+j] = m[i*size+j];
        if ( m != 0 )
          delete [] m;
        m = n;
        size = s;
      }
    }
    
    //=========================================================================
    class Set_ : public Vector {
    
            public:
              Set_( int size = STEP ) : Vector( size ) {};
    
              virtual int Add( int v );  // zwraca v, gdy pomyslnie, inaczej -1
              virtual void SetAt( int index, int v ) {};
    
              virtual int RemoveAt( int index );
    
    
    };
    
    
    
    int Set_::Add( int v )
    {
      if ( ! Contains( v ) )
        return Vector::Add( v );
      else
        return -1;
    }
    
    
    
    int Set_::RemoveAt( int index )
    {
      elements[index] = elements[num_of_elem - 1];
      return Vector::RemoveAt( num_of_elem-1 );
    }
    
    
    
    //=============================================================
    
    class Result {
    
            public:
              Result( Result *r=NULL );
    
              void SetAt( int v, int val );
              void SetAt( int v1, int v2, int val );
    
              int GetAt( int v ) const;
              int GetAt( int v1, int v2 ) const;
    
              int GetSize() const;
              int GetMaxValue() const;
              void Clear(); // wektor wynikow i tablica puste
    
              ~Result();
    
            private:
              Vector *v;
              Array  *m;
    };
    
    
    Result::Result( Result *r )
    {
      v = new Vector();
      //if ( !v )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      m = new Array();
      //if ( !m )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
    
      if ( r != NULL ) {
        int size = r->GetSize(), i, j;
    
        for ( i=0; i < size; i++ ) {
          SetAt( i, r->GetAt( i ) );
          for ( j=0; j < size; j++ )
            SetAt( i, j, r->GetAt( i, j ) );
        }
      }
    }
    
    
    
    void Result::SetAt( int v, int val )
    {
      this->v->SetAt( v, val );
    }
    
    
    void Result::SetAt( int v1, int v2, int val )
    {
      m->SetAt( v1, v2, val );
      m->SetAt( v2, v1, val );
    }
    
    
    int Result::GetAt( int v ) const
    {
      return this->v->GetAt( v );
    }
    
    
    int Result::GetAt( int v1, int v2 ) const
    {
      return m->GetAt( v1, v2 );
    }
    
    
    int Result::GetSize() const
    {
      int a = v->GetSize(), b = m->GetSize();
      return ( a > b ? a : b );
    }
    
    
    int Result::GetMaxValue() const
    {
      int max = -1;
    
      for ( int i=0, j; i < m->GetSize(); i++ )
        for ( j=0; j < m->GetSize(); j++ )
          if ( m->GetAt( i, j ) > max )
            max = m->GetAt( i, j );
      for ( int i=0; i < v->GetSize(); i++ )
        if ( v->GetAt( i ) > max )
          max = v->GetAt( i );
      return max;
    }
    
    
    void Result::Clear()
    {
      m->Clear(); // tablica bedzie pusta
      v->RemoveAll(); // wektor bedzie pusty
    }
    
    
    Result::~Result()
    {
      delete m;
      delete v;
    }
    
    
    //============================================================
    
    class Graph {
    
            public:
              Graph( int size=0 );
              Graph( const Graph *g1 ); // konstruktor kopiujacy
    
              static void CreateLineGraph( const Graph *g, const Result *r, Graph *new_g, Result *new_r );
              virtual void Clear(); // graf bedzie bez wierzcholkow, zwalnia pamiec
              void MakeEmpty(); // wypelnia macierz sasiedztwa zerami
              void EnableLabeling() { labeling_enabled = 1; }
    
              virtual void RemoveVertex( int v );
              virtual void AddVertex();
              virtual void RemoveEdge( int v1, int v2 );
              virtual void RemoveAllEdges();
              virtual void AddEdge( int v1, int v2 );
    
              virtual int AreAdjacent( int v1, int v2 ) const;
              virtual int IsEmpty() const;
              virtual int GetSize() const { return m->GetSize(); }
              virtual char *GetLabel( int v ) const;
              virtual void SetLabel( int v, char *s );
              virtual void ComputeLabels();
    
              virtual Vector *Neighbors( int v ) const;
              virtual int Degree( int v ) const;
              virtual int MaxDegree() const;
    
              virtual void MakeRandom( float density=0.5, int connected=1 );
                // domyslnie spojny
              virtual void RandomBoundedDegree( int size, int deg, double density=0.5 );
                // zawsze spojny
              void RandomSpanningTree( int start, int stop );
                // tworzy drzewo spinajace poddgraf zawierajacy wierzcholki start,...,stop-1 
              virtual void SetSize( int size ); // graf bedzie pusty
    
              virtual void SaveToFile( char *filename ) const;
              virtual void ReadFromFile( ifstream &in ); 
              virtual void ReadFromFile( char *filename ); // for reading one graph
              ~Graph();
    
            private:
              Array *m;
              char **labels;
              int labels_size;
              int labeling_enabled;
    
            protected:
              void CompLabels( int size );
              void ClearLabels();
    };
    
    void Graph::CreateLineGraph( const Graph *g, const Result *r, Graph *new_g, Result *new_r )
    {
      int size = g->GetSize();
      int k=0, i, j, i1, j1;
      Array loc;
    
      for ( i=0; i < size; i++ )
        for ( j=i+1; j <size; j++ )
          if ( g->AreAdjacent( i, j ) ) {
            loc.SetAt( i, j, k );
            k++;
          }
    
    
      new_g->SetSize( k );
      new_r->Clear();
    
      for ( i=0; i < size; i++ )
        for ( j=i+1; j < size; j++ )
          if ( g->AreAdjacent( i, j ) ) {
            new_r->SetAt( loc.GetAt( i, j ), r->GetAt( i, j ) );
            for ( i1=0; i1 < size; i1++ )
              for ( j1=i1+1; j1 < size; j1++ )
                if ( g->AreAdjacent( i1, j1 ) && g->AreAdjacent( i1, j1 ) &&
                   !( (i1==i && j1==j) || (i1==j && j1==i) ) &&
                   (i1==i || j1==j || i1==j || j1==i) ) {
                  new_g->AddEdge( loc.GetAt( i, j ), loc.GetAt( i1, j1 ) );
                }
          }
    
      char s[1024];
      for ( i=0; i < size; i++ )
        for ( j=i+1; j <size; j++ )
          if ( g->AreAdjacent( i, j ) ) {
            sprintf( s, "%s%s", g->GetLabel( i ), g->GetLabel( j ) );
            new_g->SetLabel( loc.GetAt( i, j ), s );
          }
    }
    
    Graph::Graph( int size )
    {
      m = new Array( size );
      //if ( !m )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      labels = 0;
      labels_size = 0;
      labeling_enabled = 0;
      CompLabels( size );
    }
    
    
    Graph::Graph( const Graph *g1 ) // konstruktor kopiujacy
    {
      int size = g1->GetSize(), i, j;
      m = new Array( size );
      //if ( !m )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      MakeEmpty();
    
      labels = 0;
      labels_size = 0;
      for ( i=1; i < size; i++ )
        for ( j=0; j < i; j++ )
          if ( g1->AreAdjacent( i, j ) )
            AddEdge( i, j );
      for ( i=size-1; i >= 0; i-- )
        SetLabel( i, g1->GetLabel( i ) );
    }
    
    
    void Graph::Clear()
    {
      m->Clear();
      ClearLabels();
    }
    
    
    void Graph::MakeEmpty()
    {
      int i, j, size = m->GetSize();
      for ( i=0; i < size; i++ )
        for ( j=0; j < size; j++ )
          m->SetAt( i, j, 0 );
    }
    
    
    void Graph::ClearLabels()
    {
      if ( labels == 0 )
        return;
      for ( int i=0; i < labels_size; i++ )
        if ( labels[i] )
          delete labels[i];
      delete [] labels;
      labels = 0;
      labels_size = 0;
    }
    
    
    void Graph::SetLabel( int v, char *s )
    {
      if ( !labeling_enabled )
        return;
    
      if ( v >= GetSize() || !s )
        return;
      if ( v >= labels_size ) {
        char **l = new char*[GetSize()];
        //if ( !l )
        //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
        int i;
    
        for ( i=0; i < labels_size; i++ )
          l[i] = labels[i];
        for ( i=labels_size; i < GetSize(); i++ ) {
          l[i] = new char[2];
          //if ( !l[i] )
          //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
          l[i][0] = '?';
          l[i][1] = '\0';
        }
        if ( labels )
          delete [] labels;
        labels = l;
        labels_size = GetSize();
      }
    
      if( labels[v] )
        delete labels[v];
      if ( strlen( s ) > 0 ) {
        labels[v] = new char[strlen(s)+1];
        //if ( !labels[v] )
        //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
        labels[v][strlen(s)] = '\0';
        strcpy( labels[v], s );
      } else
        labels[v] = 0;
    }
    
    
    char *Graph::GetLabel( int v ) const
    {
      if ( labels && labels_size > v )
        return labels[v];
      else
        return "?";
    }
    
    
    void Graph::ComputeLabels()
    {
      CompLabels( m->GetSize() );
    }
    
    
    void Graph::CompLabels( int size )
    {
      int i;
      char tab[4];
      tab[1] = '\0';
    
      if ( !labeling_enabled )
        return;
      if ( size == labels_size )
        return; // oblicza tylko, gdy graf sie zmienial
    
      for ( i=size-1; i >= 0; i-- ) {
        tab[0] = 'a' + i;
        SetLabel( i, tab );
      }
    }
    
    
    void Graph::RemoveVertex( int v )
    {
      m->RemoveRowAndColumn( v );
    }
    
    
    void Graph::SetSize( int size )
    {
      Clear();
      for ( int i=0; i < size; i++ )
        AddVertex();
    }
    
    
    void Graph::AddVertex()
    {
      int size = m->GetSize();
      for ( int i=0; i <= size; i++ ) {
        m->SetAt( size, i, 0 );
        m->SetAt( i, size, 0 );
        SetLabel( size, "?" );
      }
    }
    
    
    void Graph::RemoveEdge( int v1, int v2 )
    {
      m->SetAt( v1, v2, 0 );
      m->SetAt( v2, v1, 0 );
    }
    
    
    void Graph::RemoveAllEdges()
    {
      int num_of_elem = m->GetSize();
    
      for ( int i=1, j; i < num_of_elem; i++ )
        for ( j=0; j < i; j++ )
          RemoveEdge( i, j );
    }
    
    
    void Graph::AddEdge( int v1, int v2 )
    {
      m->SetAt( v1, v2, 1 );
      m->SetAt( v2, v1, 1 );
    }
    
    
    int Graph::AreAdjacent( int v1, int v2 ) const
    {
      return  m->GetAt( v1, v2 );
    }
    
    
    int Graph::IsEmpty() const
    {
      int size = m->GetSize();
    
      for ( int i=1, j; i < size; i++ )
        for ( j=0; j < i; j++ )
          if ( AreAdjacent( i, j ) )
            return 0;
      return 1;
    }
    
    
    Vector *Graph::Neighbors( int v ) const
    {
      Vector *set = new Vector();
      //if ( !set )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      int num_of_elem = m->GetSize();
    
      for ( int i=0; i < num_of_elem; i++ )
        if ( i != v && AreAdjacent( i, v ) )
          set->Add( i );
      return set;
    }
    
    
    int Graph::Degree( int v ) const
    {
      int num_of_elem = m->GetSize(), deg = 0;
    
      for ( int i=0; i < num_of_elem; i++ )
        if ( i != v && AreAdjacent( i, v ) )
          deg++;
      return deg;
    }
    
    
    int Graph::MaxDegree() const
    {
      int d=0, tmp, i, n=m->GetSize();
    
      for ( i=0; i < n; i++ ) {
        tmp = Degree( i );
        if ( tmp > d )
          d = tmp;
      }
      return d;
    }
    
    
    void Graph::RandomSpanningTree( int start, int stop )
    {
      if ( start + 1 == stop )
        return;
      else {
        int part = 1 + rand() % (stop-start-1);
        RandomSpanningTree( start, start + part );
        RandomSpanningTree( start + part, stop);
        AddEdge( start + ( rand()%part ), start + part + ( rand()%(stop-start-part) ) );
      }
    }
    
    
    void Graph::MakeRandom( float density, int connected )
    {
      int num_of_elem = m->GetSize();
      int to_add = (int) ((num_of_elem*(num_of_elem-1))*density);
      int i, j, e;
    
      Clear();
      if ( connected ) {
        if ( to_add < num_of_elem - 1 )
          to_add = num_of_elem-1;
         RandomSpanningTree( 0, num_of_elem );
         to_add -= num_of_elem - 1;
        /*
        for ( i=1; i < num_of_elem; i++ ) {
          e = rand() % i;
          AddEdge( i, e );
          to_add--;
        }
        */
      }
    
      int *v1 = new int[2*num_of_elem*num_of_elem];
      //if ( !v1 )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      int s=0;
      for ( i=1; i < num_of_elem; i++ )
        for ( j=0; j < i; j++ )
          if ( !AreAdjacent( i, j ) ) {
            v1[s++] = i;
            v1[s++] = j;
          }
    
      while ( to_add && s > 0 ) {
        i = rand() % (s/2);
        AddEdge( v1[2*i], v1[2*i+1] );
        v1[2*i] = v1[s-2];
        v1[2*i+1] = v1[s-1];
        s -= 2;
        to_add--;
      }
      delete [] v1;
    }
    
    class Tree : public Graph {
    
            public:
              //static void DrawRootedTree( Tree *t, PictureOfGraph *p, int x=-1, int y=20, int root=-1 );
    
              Tree( const Graph *g );
              Tree( const Tree *tr ); // konstruktor kopiujacy
              Tree() : Graph() { subtree_sizes = NULL; };
              ~Tree();
    
              virtual int GetRoot() const { return 0; }
              int GetSon( int u, int i ) const; // i-ty syn wierzcholka u, -1, gdy brak
                                                // numerujemy od 1
              virtual void MakeRandom( int size );
              virtual bool IsLeaf( int u ) const { return GetSon( u, 1 ) == -1; }
              virtual int NumOfSons( int u ) const;   // zwraca ilosc synow
              virtual int MaxDegree() const;
              int NumberOfSon( int son ) const;
                // odpowiada ktorym z kolei synem jest son
                // -1 gdy son nie ma ojca
              
              virtual int GetSize() const { return v.GetSize(); }
    
              virtual void Clear(); // graf bedzie pusty
    
              virtual void RemoveVertex( int v );
              virtual void AddVertex();
              virtual void RemoveEdge( int v1, int v2 );
              virtual void RemoveAllEdges();
              virtual void AddEdge( int v1, int v2 );
                // v1 bedzie ojcem v2
              virtual void EdgeContraction( int u, int w );
                // uwaga: wierzcholki o numerach max{u,w}+1,... sa
                // przesuwane o jeden w lewo w tablicy, wiec zmienia
                // sie ich indeksowanie
    
              int GetCentralVertex( int no ) const;
                // moga byc dwa centralne wierzcholki, wiec no=0,1
    
              void GetInducedSubgraph( Graph *sub, Vector &mark, Vector &mapping ) const;
               // zwraca podgraf indukowany przez wierzcholki, dla ktorych
               // mark==1. Tablica mapping zawiera odwzorowanie wierzcholkow
               // nowego grafu na wierzcholki wyjsciowego
              void GetConnectedComponent( Tree *subtree, int start_vertex, Vector &mapping ) const;
               // zwraca skl. spojnosci zawierajaca wierzcholek start_vertex
               // znaczenie mapping j.w.
    
              virtual int AreAdjacent( int v1, int v2 ) const;
              virtual int IsEmpty() const;
              int SubtreeSize( int r );
                // zwraca liczbe wierzcholkow w poddrzewie zakorzenionym w r
    
              virtual Vector *Neighbors( int v ) const;
              virtual int Degree( int v ) const;
    
              virtual void MakeRandom( float density=0.5 ); // gestosc ignorowana
              virtual void RandomBoundedDegree( int size, int deg ); // tworzy drzewo o max. stopniu rownym deg
              virtual void SetSize( int size );
    
              virtual void SaveToFile( char *filename ) const;
              virtual void ReadFromFile( ifstream &in );
    
              virtual void ComputeLabels();
              int GetHeight( int root=-1 ) const; // ilosc krawedzi na najdl
                   // sciezce od liscia do korzenia
              int GetWidth( int root=-1 ) const;
              int GetFather( int v ) const;
                   // zwraca -1, gdy v jest korzeniem
              void MakeFather( int father, int son );
                    // zamienia wierzcholki, gdy son == root
              void MakeSon( int son, int new_father, int num_of_son );
                    // wierzcholek son bedzie synem nr num_of_son wierzcholka new_father
                    // UWAGA: ZMIENIANA JEST NUMERACJA WIERZCHOLKOW,
                    // KTORE SA SYNAMI new_father
    
            protected:
              static int X_STEP_DRAW;
              static int Y_STEP_DRAW;
    
            private:
              Vector v;
              int *subtree_sizes;
              int subtree_sizes_size;
    
              void MarkConnectedComponent( Vector &mark, int start_v ) const;
                // wpisuje 1 w mark, gdy odpowiadajacy wierzcholek nalezy do skl. spojnosci
                // zaklada sie, ze inicjalnie mark jest wyzerowany
              void CreateSubtree( const Graph *g, int root );
              int HeightOfSubtree( int root ) const; // ilosc wierzch na najdl sciezce
              int WidthOfSubtree( int root ) const;
              void ExchangeVertices( int v1, int v2 );
                // zamienia wierzcholki miejscami tak,
                // aby struktura grafu pozostala niezmieniona
    };
    
    
    
    void Graph::RandomBoundedDegree( int size, int deg, double density )
    {
      int i, j, iv, jv;
      Tree t;
      int to_add = (int) ((size*(size-1))*density) - (size - 1);
    
      t.RandomBoundedDegree( size, deg );
      SetSize( size );
      for ( i=1; i < size; i++ ) {
        AddEdge( t.GetFather( i ), i );
      }
    
      int *v1 = new int[2*size*size];
      //if ( !v1 )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      int s=0;
      for ( i=1; i < size; i++ )
        if ( Degree( i ) < deg )
          for ( j=0; j < i; j++ )
            if ( !AreAdjacent( i, j ) ) {
              v1[s++] = i;
              v1[s++] = j;
            }
    
      while ( to_add > 0 && s > 0 ) {
        i = rand() % (s/2);
        if ( Degree( v1[2*i] ) < deg && Degree( v1[2*i+1] ) < deg ) {
          AddEdge( v1[2*i], v1[2*i+1] );
          to_add--;
        }
        v1[2*i] = v1[s-2];
        v1[2*i+1] = v1[s-1];
        s -= 2;
      }
      delete [] v1;
    }
    #include <fstream.h>
    void Graph::SaveToFile( char *filename ) const
    {
      ofstream f;
      int i, j, n = m->GetSize();
    
      f.open( filename, ios::ate );
      f << "n=" << n << endl;
      for ( i=0; i < n; i++ ) {
        for ( j=0; j < n-1; j++ )
          if ( AreAdjacent( i, j ) )
            f << "1 ";
          else
            f << "0 ";
        if ( AreAdjacent( i, j ) )
          f << "1" << endl;
        else
          f << "0" << endl;
      }
      f.close();
    }
    
    #include <fstream.h>
    
    void Graph::ReadFromFile( ifstream &in )
    {
      //zakladany format:
      //n=2
      //0 1
      //1 0
      char c;
      int n, i, j, a;
    
      in >> c;
      while ( c != '=' )
        in >> c;
    
      in >> n;
      SetSize( n );
      RemoveAllEdges();
      for ( i=0; i < n; i++ )
        for ( j=0; j < n; j++ ) {
          in >> a;
          if ( a == 1 )
            AddEdge( i, j );
        }
    }
    
    
    void Graph::ReadFromFile( char *filename )
    {
      ifstream in;
    
      in.open( filename, ios::in );
      ReadFromFile( in );
      in.close();
    }
    
    
    Graph::~Graph()
    {
      Clear();
      delete m;
    }
    
    
    
    //=============================================================
    
    int Tree::Y_STEP_DRAW = 20;
    int Tree::X_STEP_DRAW = 20;
    
    Tree::Tree( const Graph *g ) : Graph()
    {
      int size = g->GetSize();
      for ( int i=0; i < size; i++ )
        v.SetAt( i, -1 );
      v.SetAt( 0, 0 );
      CreateSubtree( g, 0 );
      v.SetAt( 0, -1 );
      subtree_sizes = NULL;
    }
    
    
    Tree::Tree( const Tree *tr )
    {
      for ( int i = tr->GetSize()-1; i >= 0; i-- )
        v.SetAt( i, tr->v.GetAt( i ) );
      subtree_sizes = NULL;
    }
    
    
    Tree::~Tree()
    {
      if ( subtree_sizes != NULL )
        delete [] subtree_sizes;
    }
    
    int Tree::GetFather( int v ) const
    {
      if ( v == GetRoot() )
        return -1;
      return this->v.GetAt( v );
    }
    
    
    int Tree::GetCentralVertex( int no ) const
    {
      static Vector lab;
      static Set_ remaining;
      int i, c=0, v, f, l;
    
      lab.RemoveAll();
      remaining.RemoveAll();
      for ( i=GetSize()-1; i >=0; i-- ) {
        lab.SetAt( i, 0 );
        remaining.Add( i );
      }
      
      while ( remaining.GetSize() > 2 ) {
        c += 2;
        for ( i=remaining.GetSize()-1; i >= 0; i-- ) {
          v = remaining.GetAt( i );
          f = GetFather( v );
          if ( remaining.Contains( f ) ) {
            l = lab.GetAt( f );
            if ( l < c )
              lab.SetAt( f, l+1 );
            l = lab.GetAt( v );
            if ( l < c )
              lab.SetAt( v, l+1 );
          }
        }
        for ( i=0; i < remaining.GetSize(); i++ )
          if ( lab.GetAt( remaining.GetAt( i ) ) < c ) {
            remaining.RemoveAt( i );
            i--;
          }
      }
      if ( remaining.GetSize() == 1 || no == 0 )
        return remaining.GetAt( 0 );
      else
        return remaining.GetAt( 1 );
    }
    
    
    void Tree::GetConnectedComponent( Tree *subtree, int start_vertex, Vector &mapping ) const
    {
      Vector mark;
    
      for ( int i=GetSize()-1; i >= 0; i-- )
        mark.SetAt( i, 0 );
      MarkConnectedComponent( mark, start_vertex );
      GetInducedSubgraph( subtree, mark, mapping );
    }
    
    
    void Tree::MarkConnectedComponent( Vector &mark, int start_v ) const
    {
      Vector *n;
    
      mark.SetAt( start_v, 1 );
      n = Neighbors( start_v );
      for ( int i = n->GetSize()-1; i >=0; i-- )
        if ( mark.GetAt( n->GetAt( i ) ) != 1 )
          MarkConnectedComponent( mark, n->GetAt( i ) );
      delete n;
    }
    
    
    void Tree::GetInducedSubgraph( Graph *sub, Vector &mark, Vector &mapping ) const
    {
      int i, j, c, s=GetSize(), f;
      static Vector rew;
    
      c=0;
      sub->Clear();
      mapping.RemoveAll();
      for ( i=0; i < s; i++ )
        if ( mark.GetAt( i ) == 1 ) {
          sub->AddVertex();
          mapping.SetAt( c, i );
          rew.SetAt( i, c );
          c++;
        }
    
      for ( i=1; i < s; i++ ) {
        f = GetFather( i );
        if ( mark.GetAt( i ) == 1 && mark.GetAt( f ) == 1 )
          sub->AddEdge( rew.GetAt( f ), rew.GetAt( i ) );
      }
    }
    
    
    void Tree::MakeFather( int father, int son )
    {
      v.SetAt( son, father );
      if ( son == 0 ) { // syn jest korzeniem
        ExchangeVertices( father, son );
        v.SetAt( father, 0 );
        v.SetAt( 0, -1 );
      }
    }
    
    
    void Tree::ExchangeVertices( int v1, int v2 )
    {
      char lab[1024];
      int j, s = GetSize();
      strcpy( lab, GetLabel( v1 ) );
      SetLabel( v1, GetLabel( v2 ) );
      SetLabel( v2, lab );
    
      for ( j=0; j < s; j++ )
        if ( v.GetAt( j ) == v2 )
          v.SetAt( j, s+1 );
      for ( j=0; j < s; j++ )
        if ( v.GetAt( j ) == v1 )
          v.SetAt( j, v2 );
      for ( j=0; j < s; j++ )
        if ( v.GetAt( j ) == s+1 )
          v.SetAt( j, v1 );
    }
    
    
    void Tree::MakeSon( int son, int new_father, int num_of_son )
    {
      int i;
    
      if ( son == 0 ) {
        ExchangeVertices( son, new_father );
        i = son;
        son = new_father;
        new_father = i;
      }
    
      v.SetAt( son, new_father );
      // szukamy ktorym synem jest son
      i=1;
      while ( GetSon( new_father, i ) != son )
        i++;
    
      if ( i != num_of_son ) {
        int u = GetSon( new_father, num_of_son );
        ExchangeVertices( u, son );
      }
    }
    
    
    int Tree::NumberOfSon( int son ) const
    {
      int root = GetFather( son );
      if ( root == -1 )
        return -1;
      int n = NumOfSons( root );
      for ( int i=1; i <= n; i++ )
        if ( GetSon( root, i ) == son )
          return i;
      return -1; // nie powinno sie zdarzyc!
    }
    
    
    int Tree::GetHeight( int root ) const
    {
      if ( root == -1 )
        root = GetRoot();
      return HeightOfSubtree( root ) - 1;
    }
    
    
    int Tree::GetWidth( int root ) const
    {
      if ( root == -1 )
        root = GetRoot();
      return WidthOfSubtree( root );
    }
    
    
    int Tree::HeightOfSubtree( int root ) const
    {
      int i=1, max=0, tmp, son;
    
      while ( (son = GetSon( root, i )) != -1 ) {
        tmp = HeightOfSubtree( son );
        if ( tmp > max )
          max = tmp;
        i++;
      }
      return max + 1;
    }
    
    
    int Tree::WidthOfSubtree( int root ) const
    {
      int i=1, sum=0, son;
    
      while ( (son = GetSon( root, i )) != -1 ) {
        sum += WidthOfSubtree( son );
        i++;
      }
      return sum + 1;
    }
    
    
    void Tree::CreateSubtree( const Graph *g, int root )
    {
      int i, size = g->GetSize();
    
      for ( i=0; i < size; i++ )
        if ( v.GetAt( i ) == -1 && g->AreAdjacent( i, root ) ) {
          v.SetAt( i, root );
          CreateSubtree( g, i );
        }
    }
    
    
    void Tree::ComputeLabels()
    {
      CompLabels( v.GetSize() );
    }
    
    
    void Tree::Clear()
    {
      v.RemoveAll();
      ClearLabels();
    }
    
    
    void Tree::RemoveVertex( int v_ )
    {
      for ( int i=0; i < v.GetSize(); i++ )
        if ( v.GetAt( i ) == v_ )
          v.SetAt( i, -1 );
      v.RemoveAt( v_ );
    }
    
    
    void Tree::AddVertex()
    {
      v.Add( -1 );
    }
    
    
    void Tree::RemoveEdge( int v1, int v2 )
    {
      if ( v.GetAt( v1 ) == v2 )
        v.SetAt( v1, -1 );
      else if ( v.GetAt( v2 ) == v1 )
        v.SetAt( v2, -1 );
    }
    
    
    void Tree::RemoveAllEdges()
    {
      for ( int i=1; i < v.GetSize(); i++ )
        v.SetAt( i, -1 );
    }
    
    
    void Tree::AddEdge( int v1, int v2 )
    {
      // v1 bedzie ojcem v2
      v.SetAt( v2, v1 );
    }
    
    
    void Tree::EdgeContraction( int u, int w )
    {
      int i = min( u, w ), j = max( u, w );
      int s = GetSize(), tmp;
    
      for ( int k=j; k < s-1; k++ ) {
        tmp = v.GetAt( k+1 );
        if ( tmp == j )
          v.SetAt( k, i );
        else if ( tmp < j )
          v.SetAt( k, tmp );
        else
          v.SetAt( k, tmp-1 );
      }
      v.RemoveAt( s-1 );
    }
    
    
    int Tree::AreAdjacent( int v1, int v2 ) const
    {
      if ( v.GetAt( v1 ) == v2 || v.GetAt( v2 ) == v1 )
        return 1;
      else
        return 0;
    }
    
    
    int Tree::IsEmpty() const
    {
      for ( int i=1; i < v.GetSize(); i++ )
        if ( v.GetAt( i ) != -1 )
          return 0;
      return 1;
    }
    
    
    Vector *Tree::Neighbors( int v_ ) const
    {
      Vector *ret = new Vector();
      //if ( !ret )
      //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
    
      for ( int i=1; i < v.GetSize(); i++ )
        if ( v.GetAt( i ) == v_ )
          ret->Add( i );
      if ( v.GetAt( v_ ) != -1 )
        ret->Add( v.GetAt( v_ ) );
      return ret;
    }
    
    
    int Tree::Degree( int v ) const
    {
      int d;
      Vector *r = Neighbors( v );
      d = r->GetSize();
      delete r;
      return d;
    }
    
    
    void Tree::MakeRandom( float density )
    {
      MakeRandom( v.GetSize() );
    }
    
    
    void Tree::SetSize( int size )
    {
      v.RemoveAll();
      for ( int i=size-1; i >= 0; i-- )
        v.Add( -1 );
    }
    
    
    void Tree::MakeRandom( int size )
    {
      if ( size > v.GetSize() )
        v.RemoveAll();
      for ( int i=size-1; i > 0; i-- )
        v.SetAt( i, rand() % i );
    }
    
    
    void Tree::RandomBoundedDegree( int size, int deg )
    {
      int i, r, rv;
      Set_ sm;
    
      SetSize( size );
      if ( deg < 2 )
        deg = 2;
      sm.Add( 0 );
      for ( i=1; i < size; i++ ) {
        if ( i > deg )
          r = rand() % sm.GetSize();
        else
          r = 0;
        rv = sm.GetAt( r );
        AddEdge( rv, i );
        sm.Add( i );
        if ( Degree( rv ) >= deg )
          sm.RemoveAt( r );
      }
    }
    
    
    int Tree::GetSon( int u, int i ) const
    {
      int size = v.GetSize();
      for ( int j=1, k=1; j < size; j++ )
        if ( v.GetAt( j ) == u )
          if ( k == i )
            return j;
          else
            k++;
      return -1;
    }
    
    
    int Tree::SubtreeSize( int r )
    {
      int i, s = GetSize(), v;
    
      if ( subtree_sizes != NULL && subtree_sizes_size != s ) {
        delete [] subtree_sizes;
        subtree_sizes = NULL;
      }
    
      if ( subtree_sizes == NULL ) {
        subtree_sizes = new int[s];
        //if ( subtree_sizes == NULL )
          //MessageBox( "Out of memory: function GetCentralVertex(), class Tree", "Error", MB_OK );
        subtree_sizes_size = s;
      }
    
      for ( i = s-1; i >= r; i-- )
        subtree_sizes[i] = 0;
      for ( i = s-1; i >= r; i-- ) {
        subtree_sizes[i]++;
        v = GetFather( i );
        if ( v != -1 )
          subtree_sizes[v] += subtree_sizes[i];
      }
      return subtree_sizes[r];
    }
    
    
    int Tree::NumOfSons( int u ) const
    {
      int count = 1;
    
      while ( GetSon( u, count ) != -1 )
        count++;
      return count-1;
    }
    
    
    int Tree::MaxDegree() const
    {
      int max = Degree( 0 ), size = GetSize(), tmp;
    
      for ( int i=1; i < size; i++ ) {
        tmp = Degree( i );
        if ( tmp > max )
          max = tmp;
      }
      return max;
    }
    
    
    void Tree::SaveToFile( char *filename ) const
    {
      int n = GetSize(), i;
      ofstream f;
    
      f.open( filename, ios::ate );
      f << "n=" << n << endl;
      for ( i=1; i < n-1; i++ )
        f << GetFather( i ) << " ";
    
      if ( n > 1 )
        f << GetFather( i );
      f << endl;
      f.close();
    }
    #include <fstream.h>
    void Tree::ReadFromFile( ifstream &in )
    {
      //zakladany format:
      //n=3
      //0 1
      char c;
      int n, i, a;
    
      in >> n;
      SetSize( n );
      RemoveAllEdges();
      for ( i=0; i < n-1; i++ ) {
        in >> a;
        AddEdge( a, i+1 );
      }
    }
    
    
    
    
    
    
    
    //=======================================================
    
    class Algorithm {
    
            public:
              Algorithm( Graph *g );
    
              virtual void SetGraph( Graph *g );
              void Run();
              float GetTime() const { return time; } // w sekundach
              const Result *GetResult() const{ return &res; }
    
            protected:
              Result res;
              Graph  *g;
              float  time;
    
              virtual void Prepare();
              virtual void Execute()=0;
              virtual void Clear()=0;
    
    };
    
    
    Algorithm::Algorithm( Graph *g )
    {
      this->g = 0;
      SetGraph( g );
    }
    
    
    void Algorithm::SetGraph( Graph *g )
    {
      this->g = g;
    }
    
    
    void Algorithm::Prepare()
    {
      res.Clear();
    }
    
    #include <time.h>
    
    void Algorithm::Run()
    {
      Prepare();
    
    
      Execute();
    
      
      Clear();
    }
    
    
    
    
    
    //===========================================
    
    class TreeEdgeRanking : public Algorithm {
    
            public:
              TreeEdgeRanking( Graph *g )
                    : Algorithm( g ) { t = 0; }
    
            protected:
              Tree *t;
              void ColorAll();
              
            private:
              virtual void Prepare();
              virtual void Execute();
              virtual void Clear();
    
              Set_ **visible; // widoczne kolory dla
                               // poddrzewa zaczepionego w danym wierzcholku
              Vector sons;    // pomocnicze, zawiera synow aktualnie
                               // przetwarzanego wezla
              int max_color;   // najwyzszy uzyty dotad kolor
              Vector K;
              Vector free_labels;
              int size;
    
              void ColorSubtree( int v );
    
              void ComputeSons( int v );
              int MaxConflict(); // poddrzewa zaczepione w wezlach
                                    // z tablicy Sons, wypelnionej wczesniej
                                    // funkcja ComputeSons()
              int MinVisibleLabelGraterThan( int k ); // najmniejszy widoczny kolor, wiekszy
                                    // niz k, sposrod wierzcholkow w zbiorze Sons
              void ComputeFreeLabels( int v );
              int SmallestFreeLabelBiggerThan( int k );
              int BranchContainingBiggestLabelSmallerThan( int k );
              int LexicographicallySmallestBranchInK( int max_color );
              void BranchesWithVisibleLabel( int root, int k );
              void RemoveAllSmallerLabels( Set_ *v, int label );
    };
    
    void TreeEdgeRanking::Prepare()
    {
      Algorithm::Prepare();
      if ( g ) {
        t = new Tree( g );
       // if ( !t )
        //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      }
      visible = new Set_ *[t->GetSize()];
     // if ( !visible )
     //   Application->MessageBox( "Out of memory!", NULL, MB_OK);
      for ( int i=0; i < t->GetSize(); i++ ) {
        visible[i] = new Set_();
        //if ( !visible[i] )
        //  Application->MessageBox( "Out of memory!", NULL, MB_OK);
      }
      res.Clear();
      for ( int k=0; k < t->GetSize(); k++ )
        for ( int j=0; j < t->GetSize(); j++ )
          res.SetAt( k, j, 0 );
    }
    
    
    void TreeEdgeRanking::Execute()
    {
      ColorAll();
    }
    
    
    void TreeEdgeRanking::Clear()
    {
      int i, s = t->GetSize();
      for ( i=0; i < s; i++ )
        delete visible[i];
      delete [] visible;
      delete t;
      t = 0;
    }
    
    
    void TreeEdgeRanking::ColorAll()
    {
      max_color = 0;
      ColorSubtree( 0 );
    }
    
    
    void TreeEdgeRanking::ColorSubtree( int v )
    {
      // krok 1 - inicjalizacja
      int unlabeled_branch = t->NumOfSons( v );
      int i, alfa, gamma, w, x, j, i_0, k;
    
      if ( t->IsLeaf( v ) && v != 0 )
        return;
    
      // synowie numerowani od 1
      for ( i=1; t->GetSon( v, i ) != -1; i++ )
        ColorSubtree( t->GetSon( v, i ) );
      ComputeSons( v );
      ComputeFreeLabels( v );
      K.RemoveAll();
    
      while ( unlabeled_branch > 0 ) {
        k = K.GetSize();
        // krok 2 - znalezienie max. konfliktu
        if ( K.GetSize() < 2 ) {
          alfa = MaxConflict();
          BranchesWithVisibleLabel( v, alfa );
          k = K.GetSize();
        }
    
        // krok 3 - liczenie wolnych kolorow
        gamma = MinVisibleLabelGraterThan( alfa );
        if ( gamma == -1 )
          w = k;
        else
          w = gamma - alfa - 1;
    
        if ( ( alfa > 0 && w < k-1 ) || ( alfa == 0 && w < k ) ) {
          // krok 4a - zwalnianie wolnych kolorow
          x = SmallestFreeLabelBiggerThan( gamma );
          j = BranchContainingBiggestLabelSmallerThan( x );
          if ( res.GetAt( v, j ) == 0 )
            unlabeled_branch--;
          res.SetAt( v, j, x );
          res.SetAt( j, v, x );
          if ( x > max_color )
            max_color = x;
          RemoveAllSmallerLabels( visible[j], x );
          ComputeFreeLabels( v );
          if ( !visible[j]->Contains( x ) ) //
            visible[j]->Add( x ); //
          if ( K.Contains( j ) ) //
             k--; //
          K.Remove( j );
        } else if ( alfa > 0 && w >= k-1 ) {
          // krok 4b - rozwiazanie konfliktu dla alfa > 0
          int tmp = SmallestFreeLabelBiggerThan( alfa )-1;
          i_0 = LexicographicallySmallestBranchInK( alfa );
          for ( i=0; i < K.GetSize(); i++ )
            if ( K.GetAt( i ) != i_0 ) {
              tmp++;
              res.SetAt( v, K.GetAt( i ), tmp );
              res.SetAt( K.GetAt( i ), v, tmp );
              if ( tmp > max_color )
                max_color = tmp;
              unlabeled_branch--;
              RemoveAllSmallerLabels( visible[K.GetAt( i )], tmp );
              if ( !visible[K.GetAt( i )]->Contains( tmp ) ) //
                visible[K.GetAt( i )]->Add( tmp ); //
              ComputeFreeLabels( v );
            }
          K.RemoveAll();
        } else { // alfa == 0 && w >= k
          // krok 4c - rozwiazanie konfliktu dla alfa == 0
          for ( i=0; i < K.GetSize(); i++ ) {
            int tmp = SmallestFreeLabelBiggerThan( 0 );
            res.SetAt( v, K.GetAt( i ), tmp );
            res.SetAt( K.GetAt( i ), v, tmp );
            if ( tmp > max_color )
              max_color = tmp;
            unlabeled_branch--;
            RemoveAllSmallerLabels( visible[K.GetAt( i )], tmp );
            ComputeFreeLabels( v );
         }
         K.RemoveAll();
        }
      }
      // krok 6 - obliczanie widocznych kolorow
      for ( i=0; i < sons.GetSize(); i++ ) {
        for ( j=0; j < visible[sons.GetAt( i )]->GetSize(); j++ )
          visible[v]->Add( visible[sons.GetAt( i )]->GetAt( j ) );
        visible[v]->Add( res.GetAt( sons.GetAt( i ), v ) );
      }
    }
    
    
    void TreeEdgeRanking::RemoveAllSmallerLabels( Set_ *v, int label )
    {
      for ( int i=0; i < label; i++ )
        v->Remove( i );
    }
    
    
    void TreeEdgeRanking::BranchesWithVisibleLabel( int root, int k )
    {
      K.RemoveAll();
      for ( int i=0; i < sons.GetSize(); i++ )
        if ( k > 0 ) {
          if ( visible[sons.GetAt( i )]->Contains( k ) )
            K.Add( sons.GetAt( i ) );
        } else {
          if ( res.GetAt( root, sons.GetAt( i ) ) == 0 )
            K.Add( sons.GetAt( i ) );
        }
    }
    
    
    /* oblicza synow wierzcholka k, umieszczajac
       ich w wektorze sons */
    void TreeEdgeRanking::ComputeSons( int v )
    {
      int i=1, k;
      sons.RemoveAll();
      while ( ( k=t->GetSon( v, i++ ) ) != -1 )
        sons.Add( k );
    }
    
    
    /* zwtraca max etykiete, dla ktorej wystepuje
       konflikt (poddrzewa zaczepione w wezlach w sons) */
    int TreeEdgeRanking::MaxConflict()
    {
      int c, i, count;
    
      for ( c = max_color; c > 0; c-- ) {
        for ( i=0, count=0; i < sons.GetSize(); i++ )
          if ( visible[sons.GetAt( i )]->Contains( c ) )
            count++;
        if ( count > 1 )
          return c;
        }
      return 0;
    }
    
    
    /* najmniejszy widoczny kolor, wiekszy
       niz k, sposrod wierzcholkow w zbiorze sons */
    int TreeEdgeRanking::MinVisibleLabelGraterThan( int k )
    {
      int c, i;
    
      for ( c = k+1; c <= max_color; c++ ) {
        for ( i=0; i < sons.GetSize(); i++ )
          if ( visible[sons.GetAt( i )]->Contains( c ) )
            return c;
        }
      return -1; // brak takiego koloru
    }
    
    
    /* oblicza zb_licb_nat - kolory widoczne w sons */
    void TreeEdgeRanking::ComputeFreeLabels( int v )
    {
      int i, j, tmp;
    
      free_labels.RemoveAll();
      for ( i=1; i <= max_color; i++ ) {
        for ( j=0, tmp=0; j < sons.GetSize(); j++ )
          if ( visible[sons.GetAt( j )]->Contains( i ) ) {
            tmp = 1;
            break;
          }
        if ( tmp == 0 )
          free_labels.Add( i );
      }
      for ( i=max_color + 1; i < t->GetSize(); i++ )
        free_labels.Add( i );
      for ( i=0; i < sons.GetSize(); i++ )
        free_labels.Remove( res.GetAt( v, sons.GetAt( i ) ) );
    }
    
    
    int TreeEdgeRanking::SmallestFreeLabelBiggerThan( int k )
    {
      int i;
    
      for ( i=k+1; i <= max_color; i++ )
        if ( free_labels.Contains( i ) )
          return i;
      return max_color+1;
    }
    
    
    int TreeEdgeRanking::BranchContainingBiggestLabelSmallerThan( int k )
    {
      int i, j;
      static Set_ tmp;
    
      tmp.RemoveAll();
      for ( i=k-1; i > 0 && tmp.GetSize() == 0; i-- ) {
        //for ( j=0; j < sons.GetSize(); j++ )
        //  if ( visible[sons.GetAt( j )]->Contains( i ) )
        //    break; //return sons.GetAt( j );
    
        for ( j=0; j < sons.GetSize(); j++ )
          if ( visible[sons.GetAt( j )]->Contains( i ) )
            tmp.Add( j );
      }
    
      int count;
      for ( ; i > 0 && tmp.GetSize() > 1; i-- ) {
        count = 0;
        for ( j=tmp.GetSize()-1; j >= 0; j-- )
          if ( visible[sons.GetAt( tmp.GetAt( j ) )]->Contains( i ) )
            count++;
        if ( count > 0 )
          for ( j=tmp.GetSize()-1; j >= 0; j-- )
            if ( !visible[sons.GetAt( tmp.GetAt( j ) )]->Contains( i ) )
              tmp.RemoveAt( j );
      }
      return sons.GetAt( tmp.GetAt( 0 ) );
      //return -1; // nie powinno sie zdarzyc
    }
    
    
    int TreeEdgeRanking::LexicographicallySmallestBranchInK( int max_color )
    {
      Vector tmp;
      int i, j, c = max_color, count;
    
      for ( i=0; i < K.GetSize(); i++ )
        tmp.SetAt( i, K.GetAt( i ) );
      while ( tmp.GetSize() > 1 && c > 0 ) {
        count = 0;
        for ( i=0; i < tmp.GetSize(); i++ )
          if ( visible[tmp.GetAt( i )]->Contains( c ) )
            count++;
        if ( count > 0 && count < tmp.GetSize() )
          for ( i=0; i < tmp.GetSize(); i++ )
            if ( visible[tmp.GetAt( i )]->Contains( c ) ) {
              tmp.RemoveAt( i );
              i--;
            }
        c--;
    
      }
      return tmp.GetAt( 0 );
    }
    
    
    
    
    
    int main()
    {
      TreeEdgeRanking r( 0 );
      Tree t;
      int tests;
      const Result *res;
    
      cin >> tests;
    
      for ( int i=0; i < tests; i++ ) {
        t.ReadFromFile( (ifstream &) cin );
    
        r.SetGraph( &t );
        r.Run();
        res = r.GetResult();
        cout << res->GetMaxValue() << endl;
      }
    
      return 0;
    }
    

