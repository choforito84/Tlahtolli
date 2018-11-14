using System;

namespace Tlahtolli
{
    internal class SoundStackFactory<T> : IStackFactory
    {

        private static SoundStackFactory<T> _instance;

        /// <summary>
        /// Initializes a new instance of the <see cref="IStackFactory<T>"/> class.
        /// </summary>
        /// <remarks>
        /// The constructor is declared protected as its only meant to be instantiated by <see cref="Instance"/>.
        /// </remarks>
        protected SoundStackFactory()
        {
        }

        /// <summary>
        /// Gets or sets the default <see cref="SoundStackFactory"/> instance used by <see cref="SoundStack<T>"/>.
        /// </summary>
        public static SoundStackFactory<T> Instance
        {
            get { return _instance ?? (_instance = new SoundStackFactory<T>()); }
            set { _instance = value; }
        }

        public override IStack GetStack()
        {
            return new SoundStack<T>();
        }
    }
}
