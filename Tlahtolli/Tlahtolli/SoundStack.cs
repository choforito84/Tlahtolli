using System;
using System.Collections.Generic;
using System.Linq;

namespace Tlahtolli
{
    public sealed class SoundStack<T> : IStack, IDisposable
    {
        private T[] _sounds;

        public SoundStack()
        {
            _sounds = Array.Empty<T>();
        }

        public SoundStack(int capacity)
        {
            if (capacity < 0)
                throw new ArgumentOutOfRangeException(nameof(capacity), capacity, "A SoundStack cannot be initialized with a negative capacitiy.");
            _sounds = new T[capacity];
        }

        public SoundStack(IEnumerable<T> collection)
        {
            if (collection == null)
                throw new ArgumentNullException(nameof(collection));
            _sounds = collection.ToArray();
        }

        public int Count()
        {
            return _sounds.Length;
        }

        public bool Contains(T item)
        {
            return _sounds.Contains(item);
        }

        public T Peek()
        {
            if (_sounds.Length == 0)
                throw new InvalidOperationException("There are no sounds in the stack.");
            return _sounds.Last();
        }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}
